import os

clear = lambda: os.system("cls")  # On Windows System
clear()
import serial
import time
from colorama import Fore, init, Style, Back

init(autoreset=True)
arduino = serial.Serial(port="COM9", baudrate=115200, timeout=0.5)

# Variables

flag = True  # Cambiar por una expresion que pueda cambiarse desde el arduino
selec_filtro = ["", "", ""]
received_value = ["", "", ""]
salir = False
filtros = {
    "Brazo 1": ("u", "g", "vacio"),
    "Brazo 2": ("r", "vacio"),
    "Brazo 3": ("i", "z", "vacio"),
}

# Funciones


def write_read(x):
    arduino.write(bytes(x, "utf-8"))
    time.sleep(0.05)
    data = arduino.readline()
    return data


def read():
    data = arduino.readline()
    return data


def search(list, platform):
    for i in range(len(list)):
        if list[i] == platform:
            return True
    return False


# Main

print(Style.BRIGHT + "MENÚ DE SELECCIÓN DE FILTROS")

while not salir:
    print("\nOpciones:\n")

    for key, value in filtros.items():
        print(key, " : ", value)

    while flag == True:
        for i in range(0, 3):
            selec_filtro[i] = input(
                Style.BRIGHT + "\nSelección del brazo #" + str(i + 1) + " : "
            ).lower()

            while (
                not any(x in selec_filtro[i] for x in filtros["Brazo " + str(i + 1)])
                or (len(selec_filtro[i])) != 1
            ):  # ------acepta varias veces el mismo caracter
                selec_filtro[i] = input(
                    Fore.RED
                    + "* La entrada '"
                    + selec_filtro[i]
                    + "' no es valida, ingrese una de las opciones: "
                    + Fore.RESET
                ).lower()

            received_value[i] = write_read(selec_filtro[i]).decode("utf-8")
        flag = False
    flag = True

    print(
        Fore.RESET
        + "\nConfirmación de configuración recibida del microcontrolador: "
        + str(received_value)
        + "\n"
    )

    flag_response = write_read("begin").decode("utf-8")
    print("Esperando...")
    while True:
        flag_response = read().decode("utf-8")
        if flag_response == "done":
            break
        # else:
        # print('Esperando...')

    print(
        Back.WHITE
        + Fore.BLACK
        + "\nALINEACIÓN DE FILTROS LISTA!"
        + Back.RESET
        + Fore.RESET
    )

    repetir = input("\n\nRepetir el proceso (S/N)? : ").lower()
    while not any(
        x in repetir for x in ("s", "n")
    ):  # --------------acepta varias veces el mismo caracter
        repetir = input(
            Fore.RED
            + "* La entrada no es valida, ingrese una de las opciones: "
            + Fore.RESET
        ).lower()

    if repetir == "n":
        salir = True
    else:
        selec_filtro = ["", "", ""]
        received_value = ["", "", ""]
        del repetir
        clear()
