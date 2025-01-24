import tkinter as tk
import serial
import threading

def create_progress_bar(canvas, x, y, width, height, label):
    """Tworzy pasek postępu na podanym płótnie."""
    # Tło prostokąta
    background = canvas.create_rectangle(x, y, x + width, y + height, fill="#C8C8C8", outline="black")
    # Wypełnienie prostokąta
    fill = canvas.create_rectangle(x, y, x, y + height, fill="blue", outline="")
    # Etykieta prostokąta
    canvas.create_text(x + 50, y + height // 2, text=label, fill="#C8C8C8", font=("Arial", 10, 'bold'))
    return fill

def update_progress_bar(canvas, rect, value, max_width):
    """Aktualizuje szerokość wypełnienia paska na podstawie wartości."""
    fill_width = value * max_width // 1024
    canvas.coords(rect, canvas.coords(rect)[0], canvas.coords(rect)[1], canvas.coords(rect)[0] + fill_width, canvas.coords(rect)[3])

def read_from_uart():
    """Funkcja do odczytu danych z UART i aktualizacji prostokątów w oknie."""
    global wartosc_1, wartosc_2, wartosc_3

    while True:
        try:
            data = ser.readline().decode('utf-8').strip()
            if data:
                # Podziel wartości potencjometrów po średniku
                wartosc_1, wartosc_2, wartosc_3 = map(int, data.split(';'))
                # Aktualizacja wypełnienia prostokątów
                update_progress_bar(canvas, fill_rect_1, wartosc_1, max_width)
                update_progress_bar(canvas, fill_rect_2, wartosc_2, max_width)
                update_progress_bar(canvas, fill_rect_3, wartosc_3, max_width)
        except Exception as e:
            print(f"Błąd odczytu: {e}")

# Konfiguracja połączenia UART
try:
    ser = serial.Serial('/dev/ttyACM0', baudrate=100000, timeout=0.1)
except Exception as e:
    print(f"Nie udało się otworzyć portu: {e}")
    exit()

# Okno główne aplikacji
window = tk.Tk()
window.title("UART")
window.geometry("1000x300")

# Konfiguracja płótna do rysowania
canvas = tk.Canvas(window, width=800, height=400)
canvas.pack()

# Ustawienia prostokątów
max_width = 800
fill_rect_1 = create_progress_bar(canvas, 50, 50, max_width, 30, "input 1")
fill_rect_2 = create_progress_bar(canvas, 50, 120, max_width, 30, "input 2")
fill_rect_3 = create_progress_bar(canvas, 50, 190, max_width, 30, "input 3")

# Uruchomienie wątku do odczytu UART
uart_thread = threading.Thread(target=read_from_uart, daemon=True)
uart_thread.start()

# Uruchomienie głównej pętli aplikacji
window.mainloop()
