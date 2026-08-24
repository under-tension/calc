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

## Управление службой

Установка (нужны права root):
```
sudo install -m 755 build/calc /usr/local/bin/calc
sudo install -m 755 build/calc_client /usr/local/bin/calc_client
sudo install -d -m 755 /etc/calc /var/lib/calc /var/log/calc
sudo install -m 640 .env.example /etc/calc/calc.env
sudo install -m 644 systemd/calc.service /etc/systemd/system/calc.service
sudo useradd --system --home /var/lib/calc --shell /usr/sbin/nologin calc
sudo chown -R calc:calc /var/lib/calc /var/log/calc
sudo chown root:calc /etc/calc/calc.env
sudo systemctl daemon-reload
```

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