import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import xml.etree.ElementTree as ET

# Enumerated options as dictionaries (acting like enums)
ATTACK_TYPES = {"Aggressive": "0", "Healing": "1", "Buff": "2", "Debuff": "3", "Affliction": "4"}
TARGET_TYPES = {"enemy": "0", "ally": "1", "both": "2", "self": "3"}
STAT_OBJECTIVES = {"enemy": "0", "user": "1"}
STATUS_MODES = {"fixed": "0", "totalLifePercentage": "1"}
STATUS_TYPES = {"Poison": "Poison", "Burn": "Burn", "Regeneration": "Regeneration"}
EXTRA_EFFECT_MODES = {"fixed": "0", "damage deal": "1", "totalLifePercentage": "2"}

class AttackEditor(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Attack Editor")
        self.geometry("1000x750")
        self.tree = None
        self.filepath = None
        self.attacks = []  # List of all <attack> elements from the file.
        self.current_attack_index = None

        # Create a menu
        menubar = tk.Menu(self)
        filemenu = tk.Menu(menubar, tearoff=0)
        filemenu.add_command(label="Open", command=self.load_file)
        filemenu.add_command(label="Save", command=self.save_file)
        menubar.add_cascade(label="File", menu=filemenu)
        self.config(menu=menubar)

        # Create a main paned window: left for attack selection, right for editing.
        paned = ttk.PanedWindow(self, orient=tk.HORIZONTAL)
        paned.pack(fill=tk.BOTH, expand=True)

        # Left frame: Attack list and buttons
        self.left_frame = ttk.Frame(paned, width=200)
        paned.add(self.left_frame, weight=1)
        self.create_attack_list_panel(self.left_frame)

        # Right frame: Editor for attack details
        self.editor_frame = ttk.Frame(paned)
        paned.add(self.editor_frame, weight=4)
        self.create_editor_panel(self.editor_frame)

    def create_attack_list_panel(self, parent):
        label = ttk.Label(parent, text="Attacks:")
        label.pack(padx=5, pady=5)
        self.attack_listbox = tk.Listbox(parent, width=30)
        self.attack_listbox.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        self.attack_listbox.bind("<<ListboxSelect>>", self.on_attack_select)
        # Buttons for new and delete
        btn_frame = ttk.Frame(parent)
        btn_frame.pack(padx=5, pady=5, fill=tk.X)
        ttk.Button(btn_frame, text="New Attack", command=self.new_attack).pack(side=tk.LEFT, fill=tk.X, expand=True, padx=2)
        ttk.Button(btn_frame, text="Delete Attack", command=self.delete_attack).pack(side=tk.LEFT, fill=tk.X, expand=True, padx=2)

    def create_editor_panel(self, parent):
        # Editor panel is a scrollable frame containing all the details.
        canvas = tk.Canvas(parent)
        scrollbar = ttk.Scrollbar(parent, orient="vertical", command=canvas.yview)
        self.scrollable_frame = ttk.Frame(canvas)
        self.scrollable_frame.bind("<Configure>",
            lambda e: canvas.configure(scrollregion=canvas.bbox("all")))
        canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")
        canvas.configure(yscrollcommand=scrollbar.set)
        canvas.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")

        # Create different sections:
        self.create_attack_info_frame(self.scrollable_frame)
        self.create_properties_frame(self.scrollable_frame)
        self.create_requirements_frame(self.scrollable_frame)
        self.create_basic_stats_frame(self.scrollable_frame)
        self.create_stat_mod_frame(self.scrollable_frame)
        self.create_status_mod_frame(self.scrollable_frame)
        self.create_extras_frame(self.scrollable_frame)

        # Save Changes Button added at the bottom of the editor panel.
        save_frame = ttk.Frame(self.scrollable_frame)
        save_frame.pack(pady=10)
        ttk.Button(save_frame, text="Save Attack Changes", command=self.save_current_attack).pack()

    def create_attack_info_frame(self, parent):
        frame = ttk.LabelFrame(parent, text="Attack Info")
        frame.pack(fill=tk.X, padx=5, pady=5)
        ttk.Label(frame, text="ID:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.attack_id = ttk.Entry(frame, width=25)
        self.attack_id.grid(row=0, column=1, sticky=tk.W, padx=5, pady=2)
        ttk.Label(frame, text="Name:").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.attack_name = ttk.Entry(frame, width=25)
        self.attack_name.grid(row=0, column=3, sticky=tk.W, padx=5, pady=2)
        ttk.Label(frame, text="Description:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.description = ttk.Entry(frame, width=50)
        self.description.grid(row=1, column=1, columnspan=3, sticky=tk.W, padx=5, pady=2)
        ttk.Label(frame, text="Attack Type:").grid(row=2, column=0, sticky=tk.W, padx=5, pady=2)
        self.attack_type = ttk.Combobox(frame, values=list(ATTACK_TYPES.keys()), state="readonly", width=23)
        self.attack_type.grid(row=2, column=1, sticky=tk.W, padx=5, pady=2)

    def create_properties_frame(self, parent):
        frame = ttk.LabelFrame(parent, text="Properties")
        frame.pack(fill=tk.X, padx=5, pady=5)
        ttk.Label(frame, text="Priority:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.priority = ttk.Entry(frame, width=20)
        self.priority.grid(row=0, column=1, sticky=tk.W, padx=5, pady=2)
        ttk.Label(frame, text="Accuracy:").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.accuracy = ttk.Entry(frame, width=20)
        self.accuracy.grid(row=0, column=3, sticky=tk.W, padx=5, pady=2)

    def create_requirements_frame(self, parent):
        frame = ttk.LabelFrame(parent, text="Requirements")
        frame.pack(fill=tk.X, padx=5, pady=5)
        ttk.Label(frame, text="Target Amount:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.target_amount = ttk.Entry(frame, width=20)
        self.target_amount.grid(row=0, column=1, sticky=tk.W, padx=5, pady=2)
        ttk.Label(frame, text="Target Type:").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.target_type = ttk.Combobox(frame, values=list(TARGET_TYPES.keys()), state="readonly", width=18)
        self.target_type.grid(row=0, column=3, sticky=tk.W, padx=5, pady=2)

    def create_basic_stats_frame(self, parent):
        frame = ttk.LabelFrame(parent, text="Basic Stats")
        frame.pack(fill=tk.X, padx=5, pady=5)
        ttk.Label(frame, text="Power:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.power = ttk.Entry(frame, width=20)
        self.power.grid(row=0, column=1, sticky=tk.W, padx=5, pady=2)
        ttk.Label(frame, text="Crit Rate:").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.crit_rate = ttk.Entry(frame, width=20)
        self.crit_rate.grid(row=0, column=3, sticky=tk.W, padx=5, pady=2)

    def create_stat_mod_frame(self, parent):
        frame = ttk.LabelFrame(parent, text="Stat Modifications")
        frame.pack(fill=tk.BOTH, padx=5, pady=5, expand=True)
        self.stat_mod_listbox = tk.Listbox(frame, height=4, width=70)
        self.stat_mod_listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=5, pady=5)
        btn_frame = ttk.Frame(frame)
        btn_frame.pack(side=tk.LEFT, fill=tk.Y, padx=5, pady=5)
        ttk.Button(btn_frame, text="Add", command=self.add_stat_mod, width=15).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Edit", command=self.edit_stat_mod, width=15).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Delete", command=self.delete_stat_mod, width=15).pack(fill=tk.X, pady=2)
        self.stat_mod_entries = []

    def create_status_mod_frame(self, parent):
        frame = ttk.LabelFrame(parent, text="Status Modifications")
        frame.pack(fill=tk.BOTH, padx=5, pady=5, expand=True)
        self.status_mod_listbox = tk.Listbox(frame, height=4, width=60)
        self.status_mod_listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=5, pady=5)
        btn_frame = ttk.Frame(frame)
        btn_frame.pack(side=tk.LEFT, fill=tk.Y, padx=5, pady=5)
        ttk.Button(btn_frame, text="Add", command=self.add_status_mod, width=15).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Edit", command=self.edit_status_mod, width=15).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Delete", command=self.delete_status_mod, width=15).pack(fill=tk.X, pady=2)
        self.status_mod_entries = []

    def create_extras_frame(self, parent):
        frame = ttk.LabelFrame(parent, text="Extras")
        frame.pack(fill=tk.BOTH, padx=5, pady=5, expand=True)
        # Live Dew extra-effect (fixed type "Live Dew")
        subframe1 = ttk.LabelFrame(frame, text="Live Dew (Fixed)")
        subframe1.pack(fill=tk.X, padx=5, pady=5)
        ttk.Label(subframe1, text="Amount:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.extra_amount1 = ttk.Entry(subframe1, width=20)
        self.extra_amount1.grid(row=0, column=1, sticky=tk.W, padx=5, pady=2)
        ttk.Label(subframe1, text="Mode:").grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        self.extra_mode1 = ttk.Combobox(subframe1, values=list(EXTRA_EFFECT_MODES.keys()), state="readonly", width=18)
        self.extra_mode1.grid(row=0, column=3, sticky=tk.W, padx=5, pady=2)
        ttk.Label(subframe1, text="Effectiveness:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.extra_effectiveness1 = ttk.Entry(subframe1, width=20)
        self.extra_effectiveness1.grid(row=1, column=1, sticky=tk.W, padx=5, pady=2)
        # Block Turn extra-effect (fixed type "Block Turn")
        subframe2 = ttk.LabelFrame(frame, text="Block Turn (Fixed)")
        subframe2.pack(fill=tk.X, padx=5, pady=5)
        ttk.Label(subframe2, text="Objective:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.extra_objective2 = ttk.Combobox(subframe2, values=list(STAT_OBJECTIVES.keys()), state="readonly", width=18)
        self.extra_objective2.grid(row=0, column=1, sticky=tk.W, padx=5, pady=2)

    # -------------------- File Handling & Attack List --------------------
    def load_file(self):
        filepath = filedialog.askopenfilename(filetypes=[("XML files", "*.xml")])
        if not filepath:
            return
        self.filepath = filepath
        try:
            self.tree = ET.parse(filepath)
            root = self.tree.getroot()
            self.attacks = root.findall("attack")
            if not self.attacks:
                messagebox.showerror("Error", "No <attack> elements found.")
                return
            self.refresh_attack_list()
            self.attack_listbox.selection_set(0)
            self.load_attack(0)
        except Exception as e:
            messagebox.showerror("Error", str(e))

    def refresh_attack_list(self):
        self.attack_listbox.delete(0, tk.END)
        for attack in self.attacks:
            attack_id = attack.get("id", "")
            attack_name = attack.get("name", "")
            display = f"{attack_id}: {attack_name}"
            self.attack_listbox.insert(tk.END, display)

    def on_attack_select(self, event):
        if self.attack_listbox.curselection():
            index = self.attack_listbox.curselection()[0]
            self.load_attack(index)

    def load_attack(self, index):
        self.current_attack_index = index
        attack = self.attacks[index]
        self.attack_id.delete(0, tk.END)
        self.attack_id.insert(0, attack.get("id", ""))
        self.attack_name.delete(0, tk.END)
        self.attack_name.insert(0, attack.get("name", ""))
        descr = attack.find("description")
        if descr is not None:
            self.description.delete(0, tk.END)
            self.description.insert(0, descr.get("value", ""))
        data = attack.find("data")
        if data is None:
            data = ET.SubElement(attack, "data")
        info = data.find("info")
        if info is not None:
            atype_val = info.get("attack-type", "")
            for key, val in ATTACK_TYPES.items():
                if val == atype_val:
                    self.attack_type.set(key)
        properties = data.find("properties")
        if properties is not None:
            self.priority.delete(0, tk.END)
            self.priority.insert(0, properties.get("priority", ""))
            self.accuracy.delete(0, tk.END)
            self.accuracy.insert(0, properties.get("accuracy", ""))
        requirements = data.find("requirements")
        if requirements is not None:
            self.target_amount.delete(0, tk.END)
            self.target_amount.insert(0, requirements.get("target-amount", ""))
            ttype_val = requirements.get("target-type", "")
            for key, val in TARGET_TYPES.items():
                if val == ttype_val:
                    self.target_type.set(key)
        bstats = data.find("basic-stats")
        if bstats is not None:
            power = bstats.find("power")
            if power is not None:
                self.power.delete(0, tk.END)
                self.power.insert(0, power.get("value", ""))
            crit = bstats.find("crit-rate")
            if crit is not None:
                self.crit_rate.delete(0, tk.END)
                self.crit_rate.insert(0, crit.get("value", ""))
        # Stat modifications
        self.stat_mod_entries.clear()
        stat_mod = data.find("stat-modification")
        if stat_mod is not None:
            for stat in stat_mod.findall("stat-increment"):
                self.stat_mod_entries.append(stat.attrib)
        self.refresh_stat_mod_list()
        # Status modifications
        self.status_mod_entries.clear()
        status_mod = data.find("status-modification")
        if status_mod is not None:
            for status in status_mod.findall("status-effect"):
                if "activates-instant" not in status.attrib:
                    status.attrib["activates-instant"] = "false"
                self.status_mod_entries.append(status.attrib)
        self.refresh_status_mod_list()
        # Extras
        extras = data.find("extras")
        if extras is not None:
            extra_effects = extras.findall("extra-effect")
            if len(extra_effects) > 0:
                extra1 = extra_effects[0]
                self.extra_amount1.delete(0, tk.END)
                self.extra_amount1.insert(0, extra1.get("amount", ""))
                mode_val = extra1.get("mode", "")
                for key, val in EXTRA_EFFECT_MODES.items():
                    if val == mode_val:
                        self.extra_mode1.set(key)
                self.extra_effectiveness1.delete(0, tk.END)
                self.extra_effectiveness1.insert(0, extra1.get("effectiveness", ""))
            else:
                self.clear_live_dew()
            if len(extra_effects) > 1:
                extra2 = extra_effects[1]
                self.extra_objective2.set("")
                obj_val = extra2.get("objective", "")
                for key, val in STAT_OBJECTIVES.items():
                    if val == obj_val:
                        self.extra_objective2.set(key)
            else:
                self.clear_block_turn()
        else:
            self.clear_live_dew()
            self.clear_block_turn()

    def clear_live_dew(self):
        self.extra_amount1.delete(0, tk.END)
        self.extra_mode1.set("")
        self.extra_effectiveness1.delete(0, tk.END)

    def clear_block_turn(self):
        self.extra_objective2.set("")

    def save_file(self):
        if not self.tree or not self.filepath:
            messagebox.showerror("Error", "No file loaded.")
            return
        if self.current_attack_index is None:
            messagebox.showerror("Error", "No attack selected.")
            return
        self.save_current_attack()
        self.tree.write(self.filepath)
        messagebox.showinfo("Saved", "File saved successfully.")
        self.refresh_attack_list()

    def save_current_attack(self):
        if self.current_attack_index is None:
            messagebox.showerror("Error", "No attack selected.")
            return
        attack = self.attacks[self.current_attack_index]
        attack.set("id", self.attack_id.get())
        attack.set("name", self.attack_name.get())
        descr = attack.find("description")
        if descr is None:
            descr = ET.SubElement(attack, "description")
        descr.set("value", self.description.get())
        data = attack.find("data")
        if data is None:
            data = ET.SubElement(attack, "data")
        info = data.find("info")
        if info is None:
            info = ET.SubElement(data, "info")
        info.set("attack-type", ATTACK_TYPES.get(self.attack_type.get(), "0"))
        properties = data.find("properties")
        if properties is None:
            properties = ET.SubElement(data, "properties")
        properties.set("priority", self.priority.get())
        properties.set("accuracy", self.accuracy.get())
        requirements = data.find("requirements")
        if requirements is None:
            requirements = ET.SubElement(data, "requirements")
        requirements.set("target-amount", self.target_amount.get())
        requirements.set("target-type", TARGET_TYPES.get(self.target_type.get(), "0"))
        bstats = data.find("basic-stats")
        if bstats is None:
            bstats = ET.SubElement(data, "basic-stats")
        power = bstats.find("power")
        if power is None:
            power = ET.SubElement(bstats, "power")
        power.set("value", self.power.get())
        crit = bstats.find("crit-rate")
        if crit is None:
            crit = ET.SubElement(bstats, "crit-rate")
        crit.set("value", self.crit_rate.get())
        stat_mod = data.find("stat-modification")
        if stat_mod is not None:
            for child in list(stat_mod):
                stat_mod.remove(child)
        else:
            stat_mod = ET.SubElement(data, "stat-modification")
        for entry in self.stat_mod_entries:
            stat = ET.SubElement(stat_mod, "stat-increment")
            for key, value in entry.items():
                stat.set(key, value)
        status_mod = data.find("status-modification")
        if status_mod is not None:
            for child in list(status_mod):
                status_mod.remove(child)
        else:
            status_mod = ET.SubElement(data, "status-modification")
        for entry in self.status_mod_entries:
            status = ET.SubElement(status_mod, "status-effect")
            for key, value in entry.items():
                status.set(key, value)
        extras = data.find("extras")
        if extras is None:
            extras = ET.SubElement(data, "extras")
        extra_effects = extras.findall("extra-effect")
        while len(extra_effects) < 2:
            ET.SubElement(extras, "extra-effect")
            extra_effects = extras.findall("extra-effect")
        extra1 = extra_effects[0]
        extra1.set("type", "Live Dew")
        extra1.set("amount", self.extra_amount1.get())
        extra1.set("mode", EXTRA_EFFECT_MODES.get(self.extra_mode1.get(), "0"))
        extra1.set("effectiveness", self.extra_effectiveness1.get())
        extra2 = extra_effects[1]
        extra2.set("type", "Block Turn")
        extra2.set("objective", STAT_OBJECTIVES.get(self.extra_objective2.get(), "0"))
        self.refresh_attack_list()
        messagebox.showinfo("Saved", "Attack changes saved in memory.\nRemember to use File ▸ Save to write to disk.")

    def new_attack(self):
        if self.tree is None:
            messagebox.showerror("Error", "No file loaded.")
            return
        root = self.tree.getroot()
        new_attack = ET.Element("attack", {"id": "0", "name": "NEW_ATTACK"})
        ET.SubElement(new_attack, "description", {"value": "DESCRIPTION"})
        data = ET.SubElement(new_attack, "data")
        ET.SubElement(data, "info", {"attack-type": "0"})
        ET.SubElement(data, "properties", {"priority": "0", "accuracy": "100"})
        ET.SubElement(data, "requirements", {"target-amount": "1", "target-type": "0"})
        bstats = ET.SubElement(data, "basic-stats")
        ET.SubElement(bstats, "power", {"value": "10"})
        ET.SubElement(bstats, "crit-rate", {"value": "10"})
        ET.SubElement(data, "stat-modification")
        ET.SubElement(data, "status-modification")
        extras = ET.SubElement(data, "extras")
        ET.SubElement(extras, "extra-effect")
        ET.SubElement(extras, "extra-effect")
        root.append(new_attack)
        self.attacks.append(new_attack)
        self.refresh_attack_list()
        self.attack_listbox.selection_clear(0, tk.END)
        self.attack_listbox.selection_set(tk.END)
        self.load_attack(len(self.attacks) - 1)

    def delete_attack(self):
        if self.current_attack_index is None:
            messagebox.showerror("Error", "No attack selected.")
            return
        confirm = messagebox.askyesno("Delete Attack", "Are you sure you want to delete this attack?")
        if not confirm:
            return
        attack = self.attacks[self.current_attack_index]
        parent = self.tree.getroot()
        parent.remove(attack)
        del self.attacks[self.current_attack_index]
        self.refresh_attack_list()
        if self.attacks:
            self.attack_listbox.selection_set(0)
            self.load_attack(0)
        else:
            self.current_attack_index = None
            self.clear_editor_fields()

    def clear_editor_fields(self):
        self.attack_id.delete(0, tk.END)
        self.attack_name.delete(0, tk.END)
        self.description.delete(0, tk.END)
        self.attack_type.set("")
        self.priority.delete(0, tk.END)
        self.accuracy.delete(0, tk.END)
        self.target_amount.delete(0, tk.END)
        self.target_type.set("")
        self.power.delete(0, tk.END)
        self.crit_rate.delete(0, tk.END)
        self.stat_mod_listbox.delete(0, tk.END)
        self.status_mod_listbox.delete(0, tk.END)
        self.clear_live_dew()
        self.clear_block_turn()

    # Stat Modification Handlers
    def add_stat_mod(self):
        StatModDialog(self, None)

    def edit_stat_mod(self):
        try:
            index = self.stat_mod_listbox.curselection()[0]
            data = self.stat_mod_entries[index]
            StatModDialog(self, data, index)
        except IndexError:
            messagebox.showwarning("Edit Stat Modification", "Please select an entry to edit.")

    def delete_stat_mod(self):
        try:
            index = self.stat_mod_listbox.curselection()[0]
            del self.stat_mod_entries[index]
            self.refresh_stat_mod_list()
        except IndexError:
            messagebox.showwarning("Delete Stat Modification", "Please select an entry to delete.")

    def refresh_stat_mod_list(self):
        self.stat_mod_listbox.delete(0, tk.END)
        for entry in self.stat_mod_entries:
            disp = f"Stat: {entry.get('stat', '')}, Value: {entry.get('value', '')}, Obj: {entry.get('objective', '')}, Prob: {entry.get('probability', '')}"
            self.stat_mod_listbox.insert(tk.END, disp)

    # Status Modification Handlers
    def add_status_mod(self):
        StatusModDialog(self, None)

    def edit_status_mod(self):
        try:
            index = self.status_mod_listbox.curselection()[0]
            data = self.status_mod_entries[index]
            StatusModDialog(self, data, index)
        except IndexError:
            messagebox.showwarning("Edit Status Modification", "Please select an entry to edit.")

    def delete_status_mod(self):
        try:
            index = self.status_mod_listbox.curselection()[0]
            del self.status_mod_entries[index]
            self.refresh_status_mod_list()
        except IndexError:
            messagebox.showwarning("Delete Status Modification", "Please select an entry to delete.")

    def refresh_status_mod_list(self):
        self.status_mod_listbox.delete(0, tk.END)
        for entry in self.status_mod_entries:
            disp = (f"Type: {entry.get('type', '')}, Value: {entry.get('value', '')}, Turns: {entry.get('turns', '')}, "
                    f"Mode: {entry.get('mode', '')}, ActivatesInstantly: {entry.get('activates-instant', 'false')}")
            self.status_mod_listbox.insert(tk.END, disp)

# Dialog for Stat Modifications
class StatModDialog(tk.Toplevel):
    def __init__(self, parent, data, index=None):
        super().__init__(parent)
        self.parent = parent
        self.index = index
        self.title("Edit Stat Modification" if data else "Add Stat Modification")
        self.resizable(False, False)
        ttk.Label(self, text="Value:").grid(row=0, column=0, padx=5, pady=2)
        self.value_entry = ttk.Entry(self, width=25)
        self.value_entry.grid(row=0, column=1, padx=5, pady=2)
        ttk.Label(self, text="Objective:").grid(row=1, column=0, padx=5, pady=2)
        self.objective_combo = ttk.Combobox(self, values=list(STAT_OBJECTIVES.keys()), state="readonly", width=23)
        self.objective_combo.grid(row=1, column=1, padx=5, pady=2)
        ttk.Label(self, text="Stat:").grid(row=2, column=0, padx=5, pady=2)
        self.stat_entry = ttk.Entry(self, width=25)
        self.stat_entry.grid(row=2, column=1, padx=5, pady=2)
        ttk.Label(self, text="Probability:").grid(row=3, column=0, padx=5, pady=2)
        self.prob_entry = ttk.Entry(self, width=25)
        self.prob_entry.grid(row=3, column=1, padx=5, pady=2)
        if data:
            self.value_entry.insert(0, data.get("value", ""))
            rev_obj = {v: k for k, v in STAT_OBJECTIVES.items()}
            self.objective_combo.set(rev_obj.get(data.get("objective", "0"), "enemy"))
            self.stat_entry.insert(0, data.get("stat", ""))
            self.prob_entry.insert(0, data.get("probability", ""))
        ttk.Button(self, text="OK", command=self.on_ok, width=20).grid(row=4, column=0, columnspan=2, pady=5)

    def on_ok(self):
        entry = {
            "value": self.value_entry.get(),
            "objective": STAT_OBJECTIVES.get(self.objective_combo.get(), "0"),
            "stat": self.stat_entry.get(),
            "probability": self.prob_entry.get()
        }
        if self.index is not None:
            self.parent.stat_mod_entries[self.index] = entry
        else:
            self.parent.stat_mod_entries.append(entry)
        self.parent.refresh_stat_mod_list()
        self.destroy()

# Dialog for Status Modifications
class StatusModDialog(tk.Toplevel):
    def __init__(self, parent, data, index=None):
        super().__init__(parent)
        self.parent = parent
        self.index = index
        self.title("Edit Status Modification" if data else "Add Status Modification")
        self.resizable(False, False)
        ttk.Label(self, text="Value:").grid(row=0, column=0, padx=5, pady=2)
        self.value_entry = ttk.Entry(self, width=25)
        self.value_entry.grid(row=0, column=1, padx=5, pady=2)
        ttk.Label(self, text="Turns:").grid(row=1, column=0, padx=5, pady=2)
        self.turns_entry = ttk.Entry(self, width=25)
        self.turns_entry.grid(row=1, column=1, padx=5, pady=2)
        ttk.Label(self, text="Mode:").grid(row=2, column=0, padx=5, pady=2)
        self.mode_combo = ttk.Combobox(self, values=list(STATUS_MODES.keys()), state="readonly", width=23)
        self.mode_combo.grid(row=2, column=1, padx=5, pady=2)
        ttk.Label(self, text="Objective:").grid(row=3, column=0, padx=5, pady=2)
        self.objective_combo = ttk.Combobox(self, values=list(STAT_OBJECTIVES.keys()), state="readonly", width=23)
        self.objective_combo.grid(row=3, column=1, padx=5, pady=2)
        ttk.Label(self, text="Type:").grid(row=4, column=0, padx=5, pady=2)
        self.type_combo = ttk.Combobox(self, values=list(STATUS_TYPES.keys()), state="readonly", width=23)
        self.type_combo.grid(row=4, column=1, padx=5, pady=2)
        ttk.Label(self, text="Probability:").grid(row=5, column=0, padx=5, pady=2)
        self.prob_entry = ttk.Entry(self, width=25)
        self.prob_entry.grid(row=5, column=1, padx=5, pady=2)
        ttk.Label(self, text="Activates Instant:").grid(row=6, column=0, padx=5, pady=2)
        self.activates_combo = ttk.Combobox(self, values=["true", "false"], state="readonly", width=23)
        self.activates_combo.grid(row=6, column=1, padx=5, pady=2)
        if data:
            self.value_entry.insert(0, data.get("value", ""))
            self.turns_entry.insert(0, data.get("turns", ""))
            # For the mode and objective, use reverse dictionaries
            rev_mode = {v: k for k, v in STATUS_MODES.items()}
            self.mode_combo.set(rev_mode.get(data.get("mode", "0"), "fixed"))
            rev_obj = {v: k for k, v in STAT_OBJECTIVES.items()}
            self.objective_combo.set(rev_obj.get(data.get("objective", "0"), "enemy"))
            self.type_combo.set(data.get("type", "Poison"))
            self.prob_entry.insert(0, data.get("probability", ""))
            self.activates_combo.set(data.get("activates-instant", "false"))
        else:
            self.activates_combo.set("false")
        ttk.Button(self, text="OK", command=self.on_ok, width=20).grid(row=7, column=0, columnspan=2, pady=5)

    def on_ok(self):
        entry = {
            "value": self.value_entry.get(),
            "turns": self.turns_entry.get(),
            "mode": STATUS_MODES.get(self.mode_combo.get(), "0"),
            "objective": STAT_OBJECTIVES.get(self.objective_combo.get(), "0"),
            "type": self.type_combo.get(),
            "probability": self.prob_entry.get(),
            "activates-instant": self.activates_combo.get()
        }
        if self.index is not None:
            self.parent.status_mod_entries[self.index] = entry
        else:
            self.parent.status_mod_entries.append(entry)
        self.parent.refresh_status_mod_list()
        self.destroy()

if __name__ == "__main__":
    app = AttackEditor()
    app.mainloop()
