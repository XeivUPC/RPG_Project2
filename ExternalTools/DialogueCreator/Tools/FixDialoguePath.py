import os
import re
import json
import tkinter as tk
from tkinter import filedialog, messagebox

class PathUpdaterApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Actualizador de Rutas")
        self.root.resizable(False, False)
        self.ruta_base = None
        self.archivos_seleccionados = []

        # UI Elements
        self.label_ruta_base = tk.Label(root, text="Ruta base (RPG_Project2):")
        self.label_ruta_base.grid(row=0, column=0, padx=10, pady=10, sticky="w")

        self.entry_ruta_base = tk.Entry(root, width=50)
        self.entry_ruta_base.grid(row=0, column=1, padx=10, pady=10)

        self.btn_seleccionar_ruta_base = tk.Button(root, text="Seleccionar", command=self.seleccionar_ruta_base)
        self.btn_seleccionar_ruta_base.grid(row=0, column=2, padx=10, pady=10)

        self.label_archivos = tk.Label(root, text="Archivos a arreglar:")
        self.label_archivos.grid(row=1, column=0, padx=10, pady=10, sticky="w")

        self.btn_seleccionar_archivos = tk.Button(root, text="Seleccionar archivos", 
                                                command=self.seleccionar_archivos)
        self.btn_seleccionar_archivos.grid(row=1, column=1, padx=10, pady=10)

        self.btn_arreglar_rutas = tk.Button(root, text="Arreglar rutas", 
                                           command=self.arreglar_rutas, state=tk.DISABLED)
        self.btn_arreglar_rutas.grid(row=2, column=1, padx=10, pady=20)

        self.text_resultados = tk.Text(root, height=10, width=70)
        self.text_resultados.grid(row=3, column=0, columnspan=3, padx=10, pady=10)

    def seleccionar_ruta_base(self):
        ruta = filedialog.askdirectory(title="Selecciona la carpeta RPG_Project2")
        if ruta:
            self.ruta_base = ruta
            self.entry_ruta_base.delete(0, tk.END)
            self.entry_ruta_base.insert(0, ruta)
            self.verificar_estado_botones()

    def seleccionar_archivos(self):
        archivos = filedialog.askopenfilenames(
            title="Selecciona los archivos de configuración",
            filetypes=[("CFG files", "*.cfg")]
        )
        if archivos:
            self.archivos_seleccionados = list(archivos)
            self.verificar_estado_botones()

    def verificar_estado_botones(self):
        if self.ruta_base and self.archivos_seleccionados:
            self.btn_arreglar_rutas.config(state=tk.NORMAL)
        else:
            self.btn_arreglar_rutas.config(state=tk.DISABLED)

    def arreglar_rutas(self):
        self.text_resultados.delete(1.0, tk.END)
        resultados = []
        procesados = set()
        cola = self.archivos_seleccionados.copy()

        while cola:
            archivo_actual = cola.pop(0)
            
            if archivo_actual in procesados:
                continue
                
            procesados.add(archivo_actual)
            
            try:
                # Procesar archivo principal
                nuevos_jsons = []
                
                if archivo_actual.endswith('.json'):
                    nuevos_jsons = self.procesar_json(archivo_actual)
                else:
                    nuevos_jsons = self.procesar_archivo(archivo_actual)
                
                resultados.append(f"Procesado: {archivo_actual}")
                
                # Agregar nuevos JSONs encontrados a la cola
                for json_path in nuevos_jsons:
                    if os.path.isfile(json_path) and json_path not in procesados and json_path not in cola:
                        cola.append(json_path)
                        resultados.append(f"  Descubierto: {json_path}")
                        
            except Exception as e:
                resultados.append(f"Error en {archivo_actual}: {str(e)}")

        self.text_resultados.insert(tk.END, "\n".join(resultados))
        messagebox.showinfo("Completado", "Proceso terminado. Revisa los resultados.")

    def procesar_archivo(self, archivo):
        nuevos_jsons = []
        
        with open(archivo, 'r', encoding='utf-8') as f:
            lineas = f.readlines()

        nuevas_lineas = []
        for linea in lineas:
            if "path" in linea and "RPG_Project2" in linea:
                nueva_linea, json_path = self.corregir_ruta_en_linea(linea)
                nuevas_lineas.append(nueva_linea)
                if json_path and json_path not in nuevos_jsons:
                    nuevos_jsons.append(json_path)
            else:
                nuevas_lineas.append(linea)

        with open(archivo, 'w', encoding='utf-8') as f:
            f.writelines(nuevas_lineas)
            
        return nuevos_jsons

    def procesar_json(self, archivo):
        nuevos_jsons = []
        
        with open(archivo, 'r', encoding='utf-8') as f:
            data = json.load(f)

        def recorrer(obj):
            if isinstance(obj, dict):
                for k, v in list(obj.items()):
                    if isinstance(v, str) and "RPG_Project2" in v:
                        nueva_ruta = self.corregir_ruta(v)
                        if nueva_ruta != v:
                            obj[k] = nueva_ruta
                            if nueva_ruta.lower().endswith('.json') and os.path.isfile(nueva_ruta):
                                nuevos_jsons.append(nueva_ruta)
                    else:
                        recorrer(v)
            elif isinstance(obj, list):
                for i in range(len(obj)):
                    if isinstance(obj[i], str) and "RPG_Project2" in obj[i]:
                        nueva_ruta = self.corregir_ruta(obj[i])
                        if nueva_ruta != obj[i]:
                            obj[i] = nueva_ruta
                            if nueva_ruta.lower().endswith('.json') and os.path.isfile(nueva_ruta):
                                nuevos_jsons.append(nueva_ruta)
                    else:
                        recorrer(obj[i])

        recorrer(data)
        
        with open(archivo, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=4, ensure_ascii=False)
            
        return nuevos_jsons

    def corregir_ruta(self, ruta):
        if "RPG_Project2" in ruta:
            relativa = ruta.split("RPG_Project2", 1)[1].lstrip("/\\")
            nueva = os.path.join(self.ruta_base, relativa).replace("\\", "/")
            return nueva
        return ruta

    def corregir_ruta_en_linea(self, linea):
        # Detectar rutas en estructuras tipo JSON/INI
        json_pattern = r'(\"path\"\s*:\s*)\"((?:[^\"\\]|\\.)*RPG_Project2(?:[^\"\\]|\\.)*)\"'
        ini_pattern = r'(path\s*=\s*)(\"[^\"]*RPG_Project2[^\"]*\")'
        
        # Procesar formato JSON
        match = re.search(json_pattern, linea)
        if match:
            prefix = match.group(1)
            original_path = match.group(2)
            corrected_path = self.corregir_ruta(original_path)
            return linea.replace(f'{prefix}"{original_path}"', f'{prefix}"{corrected_path}"'), corrected_path if corrected_path.lower().endswith('.json') else None

        # Procesar formato INI
        match = re.search(ini_pattern, linea)
        if match:
            prefix = match.group(1)
            original_path = match.group(2).strip('"')
            corrected_path = self.corregir_ruta(original_path)
            return f'{prefix}"{corrected_path}"\n', corrected_path if corrected_path.lower().endswith('.json') else None

        return linea, None

if __name__ == "__main__":
    root = tk.Tk()
    app = PathUpdaterApp(root)
    root.mainloop()