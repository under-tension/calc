Консольный калькулятор. Калькулятор поддерживает следующие действия: сложение, вычитание, умножение, деление и возведение в степень.

Зависимости:
1) libpq-dev >= v15.18

dev-зависимости:
1) clang-tidy >= v19
2) clang-format >= v19

Сборка проекта
```
docker compose up -d
cmake -B build
cmake --build build
cmake --build build --target install
```

Сборка проекта с запуском статического анализатора и форматера
```
docker compose up -d
cmake -B build -DENABLE_CLANG_FORMAT=ON -DENABLE_CLANG_TIDY=ON
cmake --build build
cmake --build build --target install
```

Заполнение бд
```
docker exec -it db_postgres bash
psql -d calc -U postgres -f /var/migrations/create_table_operations.sql
```

Для дебага
```
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_VALGRIND=ON
```

Пример использования
```
./calc '{"val1": 2, "val2": 4, "operation": "+"}'
```

```
./calc '{"val1": 2, "val2": 4, "operation": "^"}'
```


```
./calc '{"val1": 3, "operation": "!"}'
```

## Настройки

Параметры берутся из файла `.env` — в текущем каталоге, рядом с бинарником или
в `/etc/calc/calc.env`. Путь можно задать явно флагом `-c`. Образец со всеми
параметрами лежит в `.env.example`; значения, уже заданные в окружении, файлом
не перекрываются.

```
cp .env.example .env
```

## Запуск сервера

Без задания в аргументах сервер принимает задания по сети (по умолчанию порт
9000):
```
./calc
```

В фоновом режиме — процесс отвязывается от терминала и пишет свой номер в
PID-файл:
```
./calc -d
kill -TERM $(cat calc.pid)
```

Клиент отправляет одно задание и завершается:
```
./calc_client '{"val1": 2, "val2": 4, "operation": "+"}'
```

Без аргументов клиент читает задания построчно и остаётся на связи, пока
работает сервер:
```
./calc_client
```

## Сборка deb-пакета

```
cmake -B build
cmake --build build
cpack --config build/CPackConfig.cmake
```

Пакет `calc_1.0.0_amd64.deb` появится в корне проекта. В него входят оба
бинарника, файл службы и образец настроек — в системных путях `/usr/bin`,
`/usr/lib/systemd/system` и `/etc/calc`.

Поле сопровождающего задаётся при конфигурации:
```
cmake -B build -DCPACK_PACKAGE_CONTACT="Имя <почта>"
```

Проверка содержимого и метаданных до установки:
```
dpkg -c calc_1.0.0_amd64.deb
dpkg -I calc_1.0.0_amd64.deb
```

Установка и удаление:
```
sudo apt install ./calc_1.0.0_amd64.deb
sudo apt remove calc      # программа удаляется, настройки остаются
sudo apt purge calc       # удаляются и настройки с каталогами
```

После установки пользователь службы, каталоги `/var/lib/calc` и
`/var/log/calc` создаются автоматически, служба регистрируется в systemd.
Настройки лежат в `/etc/calc/calc.env` и помечены как конфигурационные —
обновление пакета не затрёт внесённые правки.

```
sudo nano /etc/calc/calc.env      # указать CALC_DSN
psql -d calc -U postgres -f /usr/share/calc/migrations/create_table_operations.sql
```

## Управление службой

Управление:
```
sudo systemctl start calc
sudo systemctl status calc
sudo systemctl restart calc
sudo systemctl stop calc
journalctl -u calc -f
```

Флаг `-d` со службой не используется: systemd сам уводит процесс в фон, а с
этим флагом родительский процесс завершится сразу и служба будет считаться
упавшей.

## Запуск тестов

```
cd build
./tests
ctest -R tests_memcheck -V
```