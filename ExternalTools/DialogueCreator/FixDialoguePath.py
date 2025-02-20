import os
import tkinter as tk
from tkinter import filedialog, messagebox

class PathUpdaterApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Actualizador de Rutas")

        # Evitar que la ventana se redimensione
        self.root.resizable(False, False)  # Desactiva el redimensionamiento en ambas direcciones
        
        self.ruta_base = None
        self.archivos_seleccionados = []

        # Agregar un ícono a la ventana
        try:
            self.root.iconbitmap("Heart.ico")  # Cambia "icono.ico" por la ruta de tu ícono
        except Exception as e:
            print(f"No se pudo cargar el ícono: {e}")


        # UI Elements
        self.label_ruta_base = tk.Label(root, text="Ruta base (RPG_Project2):")
        self.label_ruta_base.grid(row=0, column=0, padx=10, pady=10, sticky="w")

        self.entry_ruta_base = tk.Entry(root, width=50)
        self.entry_ruta_base.grid(row=0, column=1, padx=10, pady=10)

        self.btn_seleccionar_ruta_base = tk.Button(root, text="Seleccionar", command=self.seleccionar_ruta_base)
        self.btn_seleccionar_ruta_base.grid(row=0, column=2, padx=10, pady=10)

        self.label_archivos = tk.Label(root, text="Archivos a arreglar:")
        self.label_archivos.grid(row=1, column=0, padx=10, pady=10, sticky="w")

        self.btn_seleccionar_archivos = tk.Button(root, text="Seleccionar archivos", command=self.seleccionar_archivos)
        self.btn_seleccionar_archivos.grid(row=1, column=1, padx=10, pady=10)

        self.btn_arreglar_rutas = tk.Button(root, text="Arreglar rutas", command=self.arreglar_rutas, state=tk.DISABLED)
        self.btn_arreglar_rutas.grid(row=2, column=1, padx=10, pady=20)

        self.text_resultados = tk.Text(root, height=10, width=70)
        self.text_resultados.grid(row=3, column=0, columnspan=3, padx=10, pady=10)

    def seleccionar_ruta_base(self):
        # Abrir cuadro de diálogo para seleccionar la ruta base
        ruta = filedialog.askdirectory(title="Selecciona la carpeta RPG_Project2")
        if ruta:
            self.ruta_base = ruta
            self.entry_ruta_base.delete(0, tk.END)
            self.entry_ruta_base.insert(0, ruta)
            self.verificar_estado_botones()

    def seleccionar_archivos(self):
        # Abrir cuadro de diálogo para seleccionar archivos
        archivos = filedialog.askopenfilenames(
            title="Selecciona los archivos de configuración",
            filetypes=[("CFG files", "*.cfg"), ("INI files", "*.ini"), ("Todos los archivos", "*.*")]
        )
        if archivos:
            self.archivos_seleccionados = list(archivos)
            self.verificar_estado_botones()

    def verificar_estado_botones(self):
        # Habilitar el botón de arreglar rutas si hay una ruta base y archivos seleccionados
        if self.ruta_base and self.archivos_seleccionados:
            self.btn_arreglar_rutas.config(state=tk.NORMAL)
        else:
            self.btn_arreglar_rutas.config(state=tk.DISABLED)

    def arreglar_rutas(self):
        # Función para arreglar las rutas en los archivos seleccionados
        self.text_resultados.delete(1.0, tk.END)  # Limpiar el área de resultados
        resultados = []

        for archivo in self.archivos_seleccionados:
            try:
                # Leer el archivo como texto plano
                with open(archivo, 'r', encoding='utf-8') as f:
                    lineas = f.readlines()

                # Procesar cada línea para corregir las rutas
                nuevas_lineas = []
                for linea in lineas:
                    if "path" in linea and "RPG_Project2" in linea:
                        # Corregir la ruta
                        nueva_linea = self.corregir_ruta_en_linea(linea)
                        nuevas_lineas.append(nueva_linea)
                    else:
                        nuevas_lineas.append(linea)

                # Guardar los cambios
                with open(archivo, 'w', encoding='utf-8') as f:
                    f.writelines(nuevas_lineas)

                resultados.append(f"Archivo actualizado: {archivo}")
            except Exception as e:
                resultados.append(f"Error en {archivo}: {e}")

        # Mostrar resultados en el área de texto
        self.text_resultados.insert(tk.END, "\n".join(resultados))
        messagebox.showinfo("Completado", "Proceso terminado. Revisa los resultados.")

    def corregir_ruta_en_linea(self, linea):
        # Corregir la ruta en una línea específica
        partes = linea.split(":", 1)  # Dividir la línea en clave y valor
        if len(partes) == 2:
            clave, valor = partes
            if "RPG_Project2" in valor:
                ruta_relativa = valor.split("RPG_Project2", 1)[1].lstrip("/\\")
                nueva_ruta = os.path.join(self.ruta_base, ruta_relativa)
                nueva_ruta = nueva_ruta.replace("\\","/")
                return f"{clave}: \"{nueva_ruta}"
        return linea  # Si no, no la cambiamos

if __name__ == "__main__":
    root = tk.Tk()
    app = PathUpdaterApp(root)
    root.mainloop()