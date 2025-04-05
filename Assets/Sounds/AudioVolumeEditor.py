import os
import tkinter as tk
from tkinter import filedialog, messagebox
from tkinter import ttk
from pydub import AudioSegment

# Función que procesa los archivos de audio
def procesar_audios(archivos, volumen_cambio, formato_salida):
    for ruta_audio in archivos:
        try:
            # Cargar el archivo de audio
            audio = AudioSegment.from_file(ruta_audio)
            
            # Ajustar el volumen
            audio_modificado = audio + volumen_cambio
            
            # Crear la ruta de salida con el mismo nombre pero el formato seleccionado
            ruta_salida = os.path.splitext(ruta_audio)[0] + "." + formato_salida
            audio_modificado.export(ruta_salida, format=formato_salida)
        except Exception as e:
            messagebox.showerror("Error", f"No se pudo procesar el archivo {os.path.basename(ruta_audio)}: {str(e)}")
    
    messagebox.showinfo("Listo", "Proceso completado, los archivos han sido modificados.")

# Función para abrir el cuadro de diálogo y seleccionar archivos
def seleccionar_archivos():
    archivos = filedialog.askopenfilenames(title="Seleccionar Archivos de Audio", 
                                           filetypes=[("Archivos de Audio", "*.mp3;*.wav;*.ogg;*.flac")])
    if archivos:
        archivos_var.set("\n".join(archivos))

# Función para obtener el valor de cambio de volumen y formato de salida
def obtener_valor_volumen():
    try:
        volumen_cambio = volumen_slider.get()  # Obtenemos el cambio de volumen (positivo o negativo)
        archivos = archivos_var.get().split("\n")  # Obtenemos la lista de archivos seleccionados
        formato_salida = formato_var.get()  # Obtenemos el formato seleccionado

        if archivos:
            if formato_salida:
                procesar_audios(archivos, volumen_cambio, formato_salida)
            else:
                messagebox.showwarning("Formato no seleccionado", "Por favor, selecciona un formato de salida.")
        else:
            messagebox.showwarning("Archivos no seleccionados", "Por favor, selecciona archivos de audio.")
    except Exception as e:
        messagebox.showerror("Error", f"Ocurrió un error: {str(e)}")

# Crear la ventana principal
root = tk.Tk()
root.title("Ajuste de Volumen de Archivos de Audio")

# Variable para almacenar los archivos seleccionados
archivos_var = tk.StringVar()

# Variable para almacenar el formato de salida
formato_var = tk.StringVar()

# Etiqueta y botón para seleccionar archivos
tk.Label(root, text="Selecciona archivos de audio").pack(pady=10)
tk.Button(root, text="Seleccionar Archivos", command=seleccionar_archivos).pack(pady=5)

# Mostrar los archivos seleccionados
tk.Label(root, textvariable=archivos_var).pack(pady=5)

# Etiqueta y control deslizante para ajustar el volumen
tk.Label(root, text="Ajustar volumen (dB)").pack(pady=10)
volumen_slider = tk.Scale(root, from_=-60, to_=60, orient=tk.HORIZONTAL)
volumen_slider.set(0)  # Valor inicial de cambio de volumen (sin cambio)
volumen_slider.pack(pady=5)

# Etiqueta y menú desplegable para elegir el formato de salida
tk.Label(root, text="Selecciona el formato de salida").pack(pady=10)

# Menú para seleccionar el formato de exportación
formatos_disponibles = ["mp3", "wav", "ogg", "flac"]
formato_menu = ttk.Combobox(root, textvariable=formato_var, values=formatos_disponibles, state="readonly")
formato_menu.set("mp3")  # Formato inicial
formato_menu.pack(pady=5)

# Botón para iniciar el proceso
tk.Button(root, text="Procesar Archivos", command=obtener_valor_volumen).pack(pady=20)

# Iniciar la interfaz gráfica
root.mainloop()
