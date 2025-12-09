//// InventoryManager.hpp
//#pragma once
//#include <vector>
//#include "Item.hpp"
//
//class InventoryManager {
//public:
//    static InventoryManager& GetInstance() {
//        static InventoryManager instance;
//        return instance;
//    }
//
//    const std::vector<Item>& GetItems() const { return items; }
//
//    void AddItem(const Item& item) {
//        // merge stackable items
//        if (item.stackable) {
//            for (auto& i : items) {
//                if (i.id == item.id) {
//                    i.quantity += item.quantity;
//                    return;
//                }
//            }
//        }
//        items.push_back(item);
//    }
//
//private:
//    InventoryManager() {}
//    std::vector<Item> items;
//};
