# VSA-LAB4 — Hash Tables

Verilənlərin strukturu və alqoritmlər – 1  
Laboratoriya işi №4

## Məqsəd

C++ dilində həş cədvəllərinin qurulması, kolliziyaların həlli və yük faktorunun performansa təsirinin araşdırılması.

## Tapşırıqlar

### Task 1 — Separate Chaining və MAD

- Universitet kitabxana kataloqu.
- Polinom həş funksiyası və Horner qaydası.
- Modulo və MAD sıxışdırma üsulları.
- Dinamik böyümə, kiçilmə və Rehashing.

### Task 2 — Open Addressing

- Firewall üçün IP ünvanlarının qara siyahısı.
- Linear Probing və Quadratic Probing.
- Lazy Deletion və Rehashing.

### Task 3 — Empirik performans analizi

- 10 000 unikal açar üzərində kolliziya analizi.
- Polinom sabitlərinin və sıxışdırma üsullarının müqayisəsi.
- Müxtəlif yük faktorlarında performansın ölçülməsi.

## Layihənin strukturu

| Fayl / Qovluq | Təyinatı |
|---|---|
| `src/HashUtils.cpp` | Həş və sıxışdırma funksiyaları |
| `src/SeparateChainingHash.cpp` | Task 1 |
| `src/OpenAddressingHash.cpp` | Task 2 |
| `benchmarks/BenchmarkUtils.cpp` | Task 3 |
| `benchmarks/benchmark.cpp` | Təcrübələrin icrası |
| `include/` | Başlıq faylları |
| `tests/` | Avtomatlaşdırılmış testlər |
| `results/` | Təcrübi nəticələr |
| `main.cpp` | Əsas proqram və konsol menyusu |
| `CMakeLists.txt` | Build konfiqurasiyası |

## Test və təcrübi nəticələr

- Proqramlaşdırma dili: C++17
- Kompilyator: MSVC (x64 Release)
- Uğurlu testlər: 105/105
- Unikal açarların sayı: 10 000
- Həş cədvəlinin sabit ölçüsü: 10007
- Yük faktorları: 0.25, 0.50, 0.75, 0.90

Təcrübi nəticələr `results/` qovluğunda CSV formatında saxlanılır.

## İcra görüntüləri
## Task 1 — Separate Chaining
## Task 2 — Open Addressing
## Task 3 — Empirik performans analizi
## Avtomatlaşdırılmış testlər
