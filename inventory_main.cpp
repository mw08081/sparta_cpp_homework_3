#include <iostream>
#include <type_traits>
#include <algorithm>
#include <stdexcept>

using namespace std;

class Item
{
private:
    std::string name_;
    int price_;

public:
    // 생성자
    Item(const string &name = "Unknown", int price = 0)
        : name_(name), price_(price) {}

    // Getter
    string GetName() const { return name_; }
    int GetPrice() const { return price_; }

    // Setter
    void SetName(const string &name) { this->name_ = name; }
    void SetPrice(int price) { this->price_ = price; }

    void PrintInfo() const
    {
        cout << "[이름: " << this->name_ << ", 가격: " << this->price_ << "G]\n";
    }
};

bool compareItemsByPrice(const Item &a, const Item &b)
{
    return a.GetPrice() < b.GetPrice();
}

// 템플릿 클래스 Inventory
template <typename T, typename = std::enable_if_t<std::is_base_of<Item, T>::value>>
class Inventory
{
private:
    T *pItems_;    // 아이템 배열
    int capacity_; // 최대 저장 용량
    int size_;     // 현재 아이템 개수

private:
    // 용량 2배 확장 함수
    void Resize(int newCapacity)
    {
        if (newCapacity < this->size_)
        {
            cout << "[Inventory Resize 오류] 아이템 개수보다 적은 용량으로 resize할 수 없습니다.\n";
            return;
        }

        T *newItems = new T[newCapacity];
        for (int i = 0; i < size_; ++i)
        {
            newItems[i] = this->pItems_[i];
        }
        delete[] this->pItems_;

        this->pItems_ = newItems;
        this->capacity_ = newCapacity;
        cout << "[Inventory 확장 성공] Inventory 용량이 2배 확장되어 " << this->capacity_ << "이(가) 되었습니다.\n";
    }

public:
    // 생성자
    Inventory(int capacity = 10)
        : pItems_(nullptr), capacity_(capacity), size_(0)
    {
        if (this->capacity_ <= 0)
        {
            this->capacity_ = 1; // 최소 1 보정
        }
        this->pItems_ = new T[capacity_]; // 동적 배열 생성
    }

    // 소멸자
    ~Inventory()
    {
        delete[] this->pItems_;
    }

    // 복사 생성자
    Inventory(const Inventory<T> &other)
        : capacity_(other.capacity_), size_(other.size_)
    {
        this->capacity_ = other.capacity_;
        this->size_ = other.size_;
        this->pItems_ = new T[capacity_];
        for (int i = 0; i < size_; ++i)
        {
            this->pItems_[i] = other.pItems_[i];
        }
        cout << "인벤토리 복사 완료\n";
    }

    // 아이템 추가
    void Add(const T &item)
    {
        if (this->size_ >= this->capacity_)
        {
            cout << "[Inventory 추가 오류] Inventory용량이 가득 찼습니다!\n";
            this->Resize(this->capacity_ * 2);
        }
        this->pItems_[this->size_++] = item;
    }

    void RemoveLastItem()
    {
        if (this->size_ == 0)
        {
            cout << "[Inventory 제거 오류] Inventory가 비어있습니다.\n";
            return;
        }
        this->size_--;
    }

    // 현재 크기
    int GetSize() const { return this->size_; }

    // 최대 용량
    int GetCapacity() const { return this->capacity_; }

    // 모든 아이템 출력
    void PrintAllItems() const
    {
        if (this->size_ < 1)
        {
            cout << "[Inventory 출력 오류] Inventory가 비어있습니다.\n";
            return;
        }

        cout << "=== Inventory Items ===\n";
        for (int i = 0; i < size_; i++)
        {
            this->pItems_[i].PrintInfo();
        }
    }

    void SortItems()
    {
        sort(this->pItems_, this->pItems_ + this->size_, compareItemsByPrice);
    }
};

int main()
{
    // 인벤토리 생성 및 아이템 추가
    cout << "=== Add Items ===\n";
    Inventory<Item> inventory(5);
    inventory.Add(Item("HP postion", 500));
    inventory.Add(Item("MP postion", 300));
    inventory.Add(Item("Bow(lv 3)", 1000));
    inventory.Add(Item("Bow(lv 5)", 3000));
    inventory.Add(Item("Bow(lv 4)", 2000));

    // 출력
    cout << "Size: " << inventory.GetSize() << ", Capacity: " << inventory.GetCapacity() << '\n';
    inventory.PrintAllItems();
    cout << '\n';

    // 정렬
    cout << "=== Sort Items ===\n";
    inventory.SortItems();

    // 출력
    cout << "Size: " << inventory.GetSize() << ", Capacity: " << inventory.GetCapacity() << '\n';
    inventory.PrintAllItems();
    cout << '\n';

    // 인벤토리 확장
    cout << "=== Expand(Reszie) Items ===\n";
    inventory.Add(Item("Bow(lv 6)", 5000));

    // 출력
    cout << "Size: " << inventory.GetSize() << ", Capacity: " << inventory.GetCapacity() << '\n';
    inventory.PrintAllItems();
    cout << '\n';

    // 인벤토리 아이템 1개 제거
    cout << "=== Remove One Item ===\n";
    inventory.RemoveLastItem();

    // 출력
    cout << "Size: " << inventory.GetSize() << ", Capacity: " << inventory.GetCapacity() << '\n';
    inventory.PrintAllItems();
    cout << '\n';

    // 인벤토리 아이템 모두 제거
    cout << "=== Remove All Items ===\n";
    while (inventory.GetSize() > 0)
        inventory.RemoveLastItem();

    // 출력
    cout << "Size: " << inventory.GetSize() << ", Capacity: " << inventory.GetCapacity() << '\n';
    inventory.PrintAllItems();
    cout << '\n';

    // 아이템 추가 및 복사생성자 호출 d
    inventory.Add(Item("Bow(lv 6)", 5000));
    Inventory<Item> inventory2 = Inventory(inventory);

    // 출력(인벤토리)
    cout << "Size: " << inventory.GetSize() << ", Capacity: " << inventory.GetCapacity() << '\n';
    inventory.PrintAllItems();
    cout << '\n';

    // 출력(인벤토리2)
    cout << "Size: " << inventory2.GetSize() << ", Capacity: " << inventory2.GetCapacity() << '\n';
    inventory2.PrintAllItems();
    cout << '\n';

    return 0;
}
