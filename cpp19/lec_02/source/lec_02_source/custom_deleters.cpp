#include <functional>
#include <iostream>
#include <memory>


struct Object {};

void CustomDeleter(Object* toDelete) {
  // Реализуем любой пользовательский код для удаления объекта |toDelete|.
  std::cout << std::hex << toDelete << std::endl;
  delete toDelete;
}

void UniqueCustomDeleterSample() {
  //
  using deleter_type = std::function<void(Object*)>;

  // Создаем указатель с пользовательской функцией для освобождения памяти.
  // Первый аргумент - это указатель на созданный объект.
  // Второй - custom deleter.
  std::unique_ptr<Object, deleter_type> ptr(new Object(), CustomDeleter);

  // При вызове деструктора |ptr| вызовется функция CustomDeleter.
}

void SharedCustomDeleterSample() {
  // Создаем указатель с пользовательской функцией для освобождения памяти.
  // Отметим, что пользовательский удалитель для std::shared_ptr не является
  // частью типа.
  // Первый аргумент указатель на созданный объект.
  // Второй - custom deleter.
  std::shared_ptr<Object> ptr(new Object(), CustomDeleter);

  // При вызове деструктора |ptr| вызовется функция CustomDeleter.
}

void UniqueForArray() {
  // Можно создать unique_ptr для массива.
  // Тогда в деструкторе будет вызван delete[].
  std::unique_ptr<Object[]> objects(new Object[100]);
}