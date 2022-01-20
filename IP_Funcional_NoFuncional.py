import subprocess
import os

busquedaTarjeta = subprocess.getoutput(['ip route|awk /^default/'])
listaTarjeta = busquedaTarjeta.split()
DispositivoRed = listaTarjeta[4]
print("Dispositivo de Red: ",DispositivoRed)
busquedaIP = subprocess.getoutput(['ip addr|awk /'+DispositivoRed+'$/'])
listaIP = busquedaIP.split()
ip = listaIP[1]
print('IP: ', ip)
ipPartes = ip.split('.')
ipIncompleta = ipPartes[0]+'.'+ipPartes[1]+'.'+ipPartes[2]+'.'

IP_FUNCIONAL = []
IP_NO_FUNCIONAL = []
for contador in range(1,255):
    ipGenerada = ipIncompleta+str(contador)
    print ('----------IP Generada: '+ipGenerada+'----------')
    if(ipGenerada != ip):
        ping = os.system('ping -c 1 '+ ipGenerada)
        if(ping == 0):
            IP_FUNCIONAL.append(ipGenerada)
        else:
            IP_NO_FUNCIONAL.append(ipGenerada)

print('\n')
print('IP Funcionales: ',IP_FUNCIONAL)
print('\n')
print('IP No Funcionales: ',IP_NO_FUNCIONAL)
print('\n')
