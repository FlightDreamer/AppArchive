ArchiveApp — это консольное приложение для сжатия и распаковки файлов с использованием алгоритма Zstandard (zstd). Программа поддерживает многопоточность и может работать с большими файлами (10 ГБ и более).

Содержание:
1.Описание
2.Требования
3.Установка
4.Использование
5.Примеры
6.Лицензия

1.Описание:
ArchiveApp предоставляет простой интерфейс для работы с файлами:
Сжатие файлов с использованием алгоритма Zstandard.
Распаковка архивов, созданных с помощью Zstandard.
Возможность настройки уровня сжатия и количества потоков для улучшения производительности.

2.Требования:
Для использования программы необходимы следующие компоненты:

Операционная система: Windows, Linux или macOS.
Компилятор: Поддержка C++17 (например, MSVC, GCC или Clang).
Библиотека Zstandard (zstd) — автоматически устанавливается через vcpkg.

3.Установка:
 Установка зависимостей
Клонируйте репозиторий проекта
git clone https://github.com/FlightDreamer/AppArchive.git
cd ArchiveApp

Для сборки проекта используется vcpkg для управления зависимостями.
# Клонируйте репозиторий vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg

# Инициализация и настройка vcpkg
./bootstrap-vcpkg.sh  # Для Linux/macOS

.\bootstrap-vcpkg.bat # Для Windows

# Установите zstd
./vcpkg install zstd  # Для Linux/macOS
.\vcpkg.exe install zstd:x64-windows # Для Windows

 Сборка проекта
Создайте папку для сборки:
cd ArchiveApp
mkdir build
cd build
Настройте проект с помощью CMake:
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
/path/to - здесь ваш путь к vcpkg. Например: cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/Projects/Tests/ArchiveApp/vcpkg/scripts/buildsystems/vcpkg.cmake
Соберите проект:
cmake --build .
Исполняемый файл archiveapp.exe будет находится в build/Debug/archiveapp.exe если Windows.
Если Linux(в моём случае Ubuntu) то в build с именем archiveapp

4.Использование:
Программа поддерживает два режима работы:
Сжатие файла : myapp a <входной файл> <выходной файл> [количество потоков] [уровень сжатия]
Распаковка файла : myapp e <входной файл> <выходной файл>
Параметры:
[количество потоков] (необязательно): Количество потоков для многопоточной обработки (по умолчанию: 1).
[уровень сжатия] (необязательно): Уровень сжатия от 1 до 22 (по умолчанию: 5).

5.Примеры:
1. Сжатие файла:
./myapp a input.txt archive.zst
Сжимает файл input.txt в архив archive.zst с уровнем сжатия 5 и одним потоком.

2. Сжатие с указанием параметров:
./myapp a input.txt archive.zst 4 9
Сжимает файл input.txt в архив archive.zst, используя 4 потока и уровень сжатия 9.

3. Сжатие без указания параметров:
./myapp a input.txt archive.zst
Сжимает файл input.txt в архив archive.zst, используя 1 потока и уровень сжатия 5.

4. Распаковка файла:
./myapp e archive.zst output.txt
Распаковывает архив archive.zst в файл output.txt.

6.Лицензия:
Этот проект использует библиотеку Zstandard (zstd), которая распространяется под лицензией BSD:
Вы можете свободно использовать, изменять и распространять программное обеспечение в соответствии с условиями лицензии.

Дополнительные замечания:
Убедитесь, что все пути к vcpkg указаны правильно.
в CMakeLists.txt вписать свой путь к vcpkg.make
set(CMAKE_TOOLCHAIN_FILE "yourpath/ArchiveApp/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

Также у меня возникла проблема на linux что cmake не мог найти ZstdConfig.cmake в /AppArchive/vcpkg/installed/x64-linux/share/zstd, однако он там есть и называется zstdConfig.cmake
Для решения просто переименуйте zstdConfig.cmake на ZstdConfig.cmake
