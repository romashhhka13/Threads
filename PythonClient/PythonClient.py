import threading
from dataclasses import dataclass
import socket, struct, time
from Message import *
import ctypes


clients = []
received_messages = []


def ProcessMessages():
    while True:
        m = Message.SendMessage(MR_BROKER, MT_GETDATA)
        if m.Header.Type == MT_DATA:
            received_messages.append(f"{m.Header.From}: {m.Data}")
        elif m.Header.Type == MT_INIT:
            clients.append(m.Header.From)
        elif m.Header.Type == MT_EXIT:
            clients.remove(m.Header.From)
        else:
            time.sleep(0.2)


def Send(m: Message):

    if len(clients) == 1:
        print("Клиентов нет!")
        return

    print("Все клиенты\n50 (MR_ALL)")
    for id in clients:
        if id != m.ClientID:
            print(id)

    to = int(input("Выберите клиента: "))
    s = input("Введите сообщение: ")
    Message.SendMessage(to, MT_DATA, s)


def Client():
    m = Message.SendMessage(MR_BROKER, MT_INIT)
    if m.Header.Type == MT_CONFIRM:

        t = threading.Thread(target=ProcessMessages)
        t.start()
        clients.append(m.ClientID)
        ctypes.windll.kernel32.SetConsoleTitleW(f"{m.ClientID}")

        while True:
            print(
                "\nВыберете действие:\n1.Отправить сообщение\n2.Полученные сообщения\n"
            )
            num = int(input("Выберете позицию: "))
            if num == 1:
                Send(m)
            elif num == 2:
                print("\nПолученные сообщения:")
                for mess in received_messages:
                    print(mess)
            else:
                print("\nНеизвестная позиция!")


Client()
