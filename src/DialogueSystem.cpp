#include "DialogueSystem.h"
#include <fstream>

DialogueSystem::DialogueSystem()
{
}

DialogueSystem::~DialogueSystem()
{
}

void DialogueSystem::LoadDialogue(const map<string, DialogueNode>& new_nodes) {
    nodes = new_nodes;
    ResetDialogue();
}

void DialogueSystem::LoadDialogueFromJSON(const string& path)
{

    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo JSON." << endl;
        return;
    }

    json json_data;
    file >> json_data; // Leer el JSON
    file.close();

    nodes.clear(); // Limpiar nodos existentes

    // Recorrer cada nodo en el JSON
    for (auto& [node_id, node_data] : json_data.items()) {
        if (node_id == "__editor" || node_id == "root") continue; // Ignorar metadata

        DialogueNode node;
        node.id = node_id;
        node.text = node_data["text"]["en"]; // Usar texto en inglés por defecto
        node.next_node = node_data.value("next", "");
        node.characterName = node_data["name"];

        // Cargar opciones (choices)
        if (node_data.contains("choices")) {
            for (auto& choice_data : node_data["choices"]) {
                DialogueChoice choice;
                choice.text = choice_data["text"]["en"]; // Usar texto en inglés
                choice.next_node = choice_data["next"];
                node.choices.push_back(choice);
            }
        }

        // Cargar condiciones
        if (node_data.contains("conditions")) {
            for (auto& cond_data : node_data["conditions"]) {
                Condition cond;

                // Buscar la variable (ej: "frienship")
                for (auto& [var_name, var_data] : cond_data.items()) {
                    cond.next_node = cond_data["next"]; // Guardar el nodo siguiente
                    if (var_name == "next" || var_name == "parent") continue; // Ignorar campos especiales

                    cond.variable = var_name; // Guardar el nombre de la variable (ej: "frienship")

                    // Extraer el valor y el operador
                    if (var_data.contains("value")) {
                        if (var_data["value"].is_number()) {
                            cond.value = var_data["value"].get<float>();
                        }
                        else if (var_data["value"].is_boolean()) {
                            cond.value = var_data["value"].get<bool>();
                        }
                    }

                    if (var_data.contains("operator")) {
                        string op = var_data["operator"];
                        if (op == "equal") cond.comparison = "==";
                        else if (op == "greater") cond.comparison = ">";
                        else if (op == "less") cond.comparison = "<";
                        // Añadir más operadores si es necesario
                    }

                    
                    break; // Solo manejamos una variable por condición
                }

                node.conditions.push_back(cond);
            }
        }

        // Cargar señales
        if (node_data.contains("signals")) {
            for (auto& [signal_name, signal_data] : node_data["signals"].items()) {
                Signal signal;
                if (signal_name == "parent")
                    continue;
                signal.name = signal_name;
                if (signal_data.begin()->is_string()) {
                    signal.data = signal_data.begin()->get<string>();
                    signal.type = SignalType::String;
                }
                else if (signal_data.begin()->is_number()) {
                    signal.data = signal_data.begin()->get<float>();
                    signal.type = SignalType::Number;
                }
                else if (signal_data.begin()->is_null()) {
                    signal.data = monostate{};
                    signal.type = SignalType::Empty;
                }
                node.signals.push_back(signal);
            }
        }

        nodes[node_id] = node;
    }

    // Establecer el nodo raíz
    if (json_data.contains("root")) {
        current_node = json_data["root"]["next"];
    }
}

void DialogueSystem::StartDialogue() {
    dialogue_active = true;
}

void DialogueSystem::Update() {
    if (!dialogue_active) return;

    DialogueNode& node = nodes[current_node];

    // Llamar señales solo al entrar al nodo
    if (current_node != previous_node) {
        TriggerCallbacks(onDialogStart);
        ProcessSignals();
        previous_node = current_node;
        TriggerCallbacks(onDialogNodeChange);
    }
}

void DialogueSystem::ProcessInput(int choice) {
    if (!dialogue_active) return;

    DialogueNode& node = nodes[current_node];

    // Verificar condiciones primero
    for (auto& condition : node.conditions) {
        if (CheckCondition(condition)) {
            current_node = condition.next_node;
            return;
        }
    }

    // Procesar elección
    if (!node.choices.empty() && choice >= 0 && choice < node.choices.size()) {
        current_node = node.choices[choice].next_node;
    }
    else if (!node.next_node.empty()) {
        current_node = node.next_node;
    }
    else {
         TriggerCallbacks(onDialogStart);
        EndDialogue();
    }

    // Marcar completado
    node.completed = true;
}

void DialogueSystem::ProcessSignals() {
    // Limpiar señales previas y activar nuevas
    active_signals.clear();

    DialogueNode& node = nodes[current_node];
    for (Signal& signal : node.signals) {
        // Determinar tipo automáticamente
        signal.type = DetermineSignalType(signal.data);
        active_signals.push_back(signal);
        TriggerCallbacks(onSignalCall, &signal);
    }
}

SignalType DialogueSystem::DetermineSignalType(const SignalData& data) {
    if (holds_alternative<monostate>(data)) return SignalType::Empty;
    if (holds_alternative<string>(data)) return SignalType::String;
    if (holds_alternative<float>(data)) return SignalType::Number;
    if (holds_alternative<Vector2>(data)) return SignalType::Vector2;
    return SignalType::Empty; // Asume que Empty es el último caso
}

bool DialogueSystem::CheckCondition(const Condition& cond) {
    auto& var = game_state[cond.variable];

    if (holds_alternative<bool>(var) && holds_alternative<bool>(cond.value)) {
        return get<bool>(var) == get<bool>(cond.value);
    }
    else if (holds_alternative<float>(var) && holds_alternative<float>(cond.value)) {
        float game_val = get<float>(var);
        float cond_val = get<float>(cond.value);

        if (cond.comparison == "==") return game_val == cond_val;
        if (cond.comparison == ">") return game_val > cond_val;
        if (cond.comparison == "<") return game_val < cond_val;
        // ... otras comparaciones
    }
    return false;
}

void DialogueSystem::EndDialogue() {
    dialogue_active = false;
    cout << "Diálogo completado!\n";
}

void DialogueSystem::ResetDialogue() {
    for (auto& [id, node] : nodes) {
        node.completed = false;
    }
    current_node = "root";
    dialogue_active = false;
}

const DialogueNode& DialogueSystem::GetCurrentDialogue()
{
    return nodes[current_node];
}

void DialogueSystem::AddGameStateVariable(const string& variable_name, const variant<bool, float>& value) {
    // Añadir o actualizar la variable en game_state
    game_state[variable_name] = value;
    cout << "Variable añadida/actualizada: " << variable_name << endl;
}

void DialogueSystem::RemoveGameStateVariable(const string& variable_name) {
    // Buscar la variable en game_state
    auto it = game_state.find(variable_name);
    if (it != game_state.end()) {
        game_state.erase(it); // Eliminar la variable
        cout << "Variable eliminada: " << variable_name << endl;
    }
    else {
        cout << "Variable no encontrada: " << variable_name << endl;
    }
}

// Getters
bool DialogueSystem::IsDialogueActive() const {
    return dialogue_active;
}

const vector<Signal>& DialogueSystem::GetActiveSignals() const {
    return active_signals;
}

void DialogueSystem::TriggerCallbacks(vector<function<void()>>& callbacks)
{
    for (auto& callback : callbacks) {
        callback();
    }
}

void DialogueSystem::TriggerCallbacks(vector<function<void(Signal*)>>& callbacks, Signal* _value)
{
    for (auto& callback : callbacks) {
        callback(_value);
    }
}