import os
import json
import tkinter as tk
from tkinter import filedialog, messagebox

class CFGtoJSONConverterApp:
    def __init__(self, root):
        self.root = root
        self.root.title("CFG to JSON Converter")
        self.root.resizable(False, False)
        self.cfg_files = []
        self.destination_folder = None

        # UI Elements
        self.label_cfg_files = tk.Label(root, text="Seleccionar archivos .cfg:")
        self.label_cfg_files.grid(row=0, column=0, padx=10, pady=10, sticky="w")

        self.btn_seleccionar_cfg = tk.Button(root, text="Seleccionar archivos .cfg", command=self.seleccionar_cfg_files)
        self.btn_seleccionar_cfg.grid(row=0, column=1, padx=10, pady=10)

        self.label_destino = tk.Label(root, text="Seleccionar carpeta de destino para .json:")
        self.label_destino.grid(row=1, column=0, padx=10, pady=10, sticky="w")

        self.btn_seleccionar_destino = tk.Button(root, text="Seleccionar carpeta", command=self.seleccionar_destino)
        self.btn_seleccionar_destino.grid(row=1, column=1, padx=10, pady=10)

        self.btn_convertir = tk.Button(root, text="Convertir a JSON", command=self.convertir_a_json, state=tk.DISABLED)
        self.btn_convertir.grid(row=2, column=1, padx=10, pady=20)

        self.text_resultados = tk.Text(root, height=10, width=70)
        self.text_resultados.grid(row=3, column=0, columnspan=3, padx=10, pady=10)

    def seleccionar_cfg_files(self):
        self.cfg_files = filedialog.askopenfilenames(
            title="Selecciona los archivos .cfg",
            filetypes=[("CFG files", "*.cfg")]
        )
        if self.cfg_files:
            self.verificar_estado_botones()

    def seleccionar_destino(self):
        self.destination_folder = filedialog.askdirectory(title="Selecciona la carpeta de destino para los archivos .json")
        if self.destination_folder:
            self.verificar_estado_botones()

    def verificar_estado_botones(self):
        if self.cfg_files and self.destination_folder:
            self.btn_convertir.config(state=tk.NORMAL)
        else:
            self.btn_convertir.config(state=tk.DISABLED)

    def convertir_a_json(self):
        self.text_resultados.delete(1.0, tk.END)
        resultados = []

        for cfg_file in self.cfg_files:
            try:
                # Parse the .cfg file
                config_dict = self.parse_cfg_file(cfg_file)

                # Generate the output JSON file path
                file_name = os.path.basename(cfg_file).replace(".cfg", ".json")
                json_path = os.path.join(self.destination_folder, file_name)

                # Save the JSON structure to a file
                with open(json_path, 'w', encoding='utf-8') as json_file:
                    json.dump(config_dict, json_file, indent=4, ensure_ascii=False)

                resultados.append(f"Convertido: {cfg_file} -> {json_path}")
            except Exception as e:
                resultados.append(f"Error en {cfg_file}: {str(e)}")

        self.text_resultados.insert(tk.END, "\n".join(resultados))
        messagebox.showinfo("Completado", "Conversión terminada. Revisa los resultados.")

    def parse_cfg_file(self, cfg_path):
        """Parse the .cfg file and return a dictionary."""
        config = {}
        current_section = None
        multiline_value = None

        with open(cfg_path, 'r', encoding='utf-8') as file:
            for line in file:
                line = line.strip()
                if not line or line.startswith('#'):  # Skip empty lines and comments
                    continue

                # Check if the line is a section header
                if line.startswith('[') and line.endswith(']'):
                    current_section = line[1:-1]
                    config[current_section] = {}
                    continue

                # Handle multiline JSON-like structures
                if multiline_value is not None:
                    if line.endswith(']') or line.endswith('}'):
                        multiline_value += line
                        try:
                            # Parse the multiline JSON-like structure
                            config[current_section][multiline_key] = json.loads(multiline_value)
                        except json.JSONDecodeError:
                            # If it's not valid JSON, keep it as a string
                            config[current_section][multiline_key] = multiline_value
                        multiline_value = None
                    else:
                        multiline_value += line
                    continue

                # Parse key-value pairs
                if '=' in line:
                    key, value = line.split('=', 1)
                    key = key.strip()
                    value = value.strip()

                    # Remove extra quotes around values
                    if value.startswith('"') and value.endswith('"'):
                        value = value[1:-1]

                    # Check if the value is the start of a multiline JSON-like structure
                    if value.startswith('[') or value.startswith('{'):
                        if not (value.endswith(']') or value.endswith('}')):
                            multiline_value = value
                            multiline_key = key
                            continue

                    # Add the key-value pair to the current section
                    config[current_section][key] = value

        return config

if __name__ == "__main__":
    root = tk.Tk()
    app = CFGtoJSONConverterApp(root)
    root.mainloop()