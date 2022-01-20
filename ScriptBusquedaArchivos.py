import os, subprocess

carpetas = ['PDF','PNG','C'] #agregar nombres de las carpetas necesarias.
extenciones = ['pdf','png','c'] #agregar las extenciones a buscar.
archivosTipo = ['ArchivosPDF','ArchivosPNG','ArchivosC'] #agregar Archivos___ que van a buscar
ruta = '/home/hugo_andres_mm/Escritorio/' #cambiar la ruta en donde se van a buscar los archivos y donde se crearan las carpetas

for numeroCarpetas in range(0,len(carpetas)):
    prueba = os.makedirs(ruta+carpetas[numeroCarpetas], exist_ok=True)    
for numeroExtensiones in range(0,len(extenciones)):
    lista = subprocess.getoutput(['find '+ruta+' -name "*."' + extenciones[numeroExtensiones]] )
    archivosTipo[numeroExtensiones] = lista.split('\n')
    for numeroArchivo in range(len(archivosTipo[numeroExtensiones])):
        archivo = archivosTipo[numeroExtensiones][numeroArchivo]
        rutaArchivo = ruta+carpetas[numeroExtensiones]
        subprocess.call(['cp',archivo,rutaArchivo])

#En caso de que sean mas carpetas, solo agregarlas ejemplo: ...,ruta+carpetas[n]
subprocess.call(['tar','-czvf',ruta+'ArchivosTodos.tar',ruta+carpetas[0],ruta+carpetas[1],ruta+carpetas[2]])
