cmake_minimum_required(VERSION 3.10)
project(archiveapp)

set(CMAKE_CXX_STANDARD 17)

# Используем vcpkg для поиска библиотек
set(CMAKE_TOOLCHAIN_FILE "C:/Projects/ArchiveApp/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Подключение библиотеки Zstandard
find_package(Zstd REQUIRED)

# Добавление исполняемого файла
add_executable(archiveapp src/main.cpp src/zstd_utils.cpp)

# Подключение библиотек
target_link_libraries(archiveapp PRIVATE zstd::libzstd)

# Установка директории для заголовочных файлов
target_include_directories(archiveapp PRIVATE ${PROJECT_SOURCE_DIR}/include)
