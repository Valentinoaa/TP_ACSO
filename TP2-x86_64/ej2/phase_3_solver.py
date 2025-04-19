
def suma_for_word(word, palabras):
    acc = 0
    low, high = 0, len(palabras) - 1
    while True:
        if low > high:
            return None
        mid = ((low ^ high) >> 1) + (low & high)
        ptr = palabras[mid]
        acc += ord(ptr[0])
        if word == ptr:
            return acc
        elif word < ptr:
            high = mid - 1
        else:
            low = mid + 1

with open("palabras.txt", "r", encoding="utf-8") as f:
    palabras = sorted(line.strip() for line in f if line.strip())

valid = []
for w in palabras:
    s = suma_for_word(w, palabras)
    if s is not None and s <= 799:
        valid.append((w, s))

for w, s in valid:
    print(f"{w}: {s}")

