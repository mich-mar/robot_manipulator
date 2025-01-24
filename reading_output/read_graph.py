import serial
import pygame
import math

# Konfiguracja połączenia
ser = serial.Serial('/dev/ttyACM0', baudrate=100000, timeout=0.1)

# Konfiguracja okna pygame
pygame.init()
szerokosc, wysokosc = 800, 800
okno = pygame.display.set_mode((szerokosc, wysokosc))
pygame.display.set_caption("Potencjometr")
font = pygame.font.SysFont(None, 30)
clock = pygame.time.Clock()

# Zmienna do przechowywania poprzedniej wartości oraz min/max wartości
poprzednia_wartosc_1 = None
poprzednia_wartosc_2 = None
min_wartosc_1 = int(100)
max_wartosc_1 = int(900)
min_wartosc_2 = int(100)
max_wartosc_2 = int(900)


# Funkcja do skalowania wartości
def skaluj_wartosc(wartosc, min_wartosc, max_wartosc):
    try:
        wartosc = float(wartosc)  # Konwersja na float
    except ValueError:
        return 0, min_wartosc, max_wartosc  # Jeśli nie uda się przekonwertować, zwróć 0

    if wartosc < min_wartosc:
        min_wartosc = wartosc
    if wartosc > max_wartosc:
        max_wartosc = wartosc

    # Skalowanie wartości na zakres 0-100
    if max_wartosc != min_wartosc:
        wartosc_skalowana = (wartosc - min_wartosc) / (max_wartosc - min_wartosc) * 100
        return wartosc_skalowana, min_wartosc, max_wartosc
    else:
        return 0, min_wartosc, max_wartosc  # Jeśli min_wartosc == max_wartosc, zwróć 0


def rysuj_wskazowke(okno, wartosc_1, wartosc_2):
    global poprzednia_wartosc_1, poprzednia_wartosc_2
    global min_wartosc_1, max_wartosc_1, min_wartosc_2, max_wartosc_2

    okno.fill((255, 255, 255))  # Czyszczenie tła na biało

    # Skaluje wartość wejściową do procentów (0-100%)
    wartosc_1_skalowana, min_wartosc_1, max_wartosc_1 = skaluj_wartosc(wartosc_1, min_wartosc_1, max_wartosc_1)
    wartosc_2_skalowana, min_wartosc_2, max_wartosc_2 = skaluj_wartosc(wartosc_2, min_wartosc_2, max_wartosc_2)

    # Jeśli fluktuacja wartości jest mniejsza niż 0.5%, nie aktualizujemy wskazówki
    if poprzednia_wartosc_1 is not None:
        roznica_1 = abs(wartosc_1_skalowana - poprzednia_wartosc_1)
        roznica_2 = abs(wartosc_2_skalowana - poprzednia_wartosc_2)
        if roznica_1 < 0.5 and roznica_2 < 0.5:
            return  # Jeśli zmiana mniejsza niż 0.5%, nie rysuj

    # Konwersja na kąt (zakres 0-360 stopni) dla obu potencjometrów
    kat_koncowy_1 = int(wartosc_1_skalowana * 360 / 100 - 90)  # Przemiana na kąt
    kat_koncowy_2 = int(wartosc_2_skalowana * 360 / 100 - 90)  # Przemiana na kąt

    # Rysowanie tła (okrągłego manometru)
    pygame.draw.circle(okno, (200, 200, 200), (szerokosc // 4, wysokosc // 2), 150)
    pygame.draw.circle(okno, (200, 200, 200), (3 * szerokosc // 4, wysokosc // 2), 150)

    # Rysowanie wskazówki dla pierwszego potencjometru
    kat_1 = 90 - kat_koncowy_1  # Ustalamy początek wskazówki na 90 stopniach
    x_1 = szerokosc // 4 + int(140 * math.sin(math.radians(kat_1)))
    y_1 = wysokosc // 2 + int(140 * math.cos(math.radians(kat_1)))

    # Rysowanie wskazówki dla drugiego potencjometru
    kat_2 = 90 - kat_koncowy_2  # Ustalamy początek wskazówki na 90 stopniach
    x_2 = 3 * szerokosc // 4 + int(140 * math.sin(math.radians(kat_2)))
    y_2 = wysokosc // 2 + int(140 * math.cos(math.radians(kat_2)))

    # Rysowanie wskazówek
    pygame.draw.line(okno, (0, 0, 0), (szerokosc // 4, wysokosc // 2), (x_1, y_1), 5)
    pygame.draw.line(okno, (0, 0, 0), (3 * szerokosc // 4, wysokosc // 2), (x_2, y_2), 5)

    # Wyświetlanie tekstu z procentem
    tekst_1 = font.render(f"Wartość 1: {wartosc_1_skalowana:.1f}%", True, (0, 0, 0))
    tekst_2 = font.render(f"Wartość 2: {wartosc_2_skalowana:.1f}%", True, (0, 0, 0))

    okno.blit(tekst_1, (szerokosc // 4 - tekst_1.get_width() // 2, wysokosc * 0.05 - tekst_1.get_height() // 2))
    okno.blit(tekst_2, (3 * szerokosc // 4 - tekst_2.get_width() // 2, wysokosc * 0.05 - tekst_2.get_height() // 2))

    pygame.display.update()  # Zaktualizuj tylko zmienioną część ekranu

    # Zapisz aktualną wartość jako poprzednią na następny cykl
    poprzednia_wartosc_1 = wartosc_1_skalowana
    poprzednia_wartosc_2 = wartosc_2_skalowana


# Główna pętla programu
dziala = True
while dziala:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            dziala = False

    # Odczyt danych z portu szeregowego
    data = ser.readline().decode('utf-8').strip()
    if data:
        # Podziel wartości potencjometrów po średniku
        wartosc_1, wartosc_2 = data.split(';')
        rysuj_wskazowke(okno, wartosc_1, wartosc_2)

    #clock.tick(60)  # Limitowanie liczby klatek na sekundę do 60

pygame.quit()
