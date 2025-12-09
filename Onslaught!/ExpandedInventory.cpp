//// InventoryUIExpanded.cpp
//#include "ExpandedInventory.hpp"
//#include "InventoryManager.hpp"
//
//InventoryUIExpanded::InventoryUIExpanded() {
//    background.setSize({600.f, 500.f});
//    background.setFillColor(sf::Color(40, 40, 40, 220));
//    background.setPosition( {50.f, 50.f} );
//
//    slotShape.setSize({80.f, 80.f});
//    slotShape.setFillColor(sf::Color(100, 100, 100));
//    slotShape.setOutlineColor(sf::Color::Black);
//    slotShape.setOutlineThickness(2.f);
//}
//
//void InventoryUIExpanded::Update() {}
//
//void InventoryUIExpanded::Draw(sf::RenderWindow& window) {
//    window.draw(background);
//
//    const auto& items = InventoryManager::GetInstance().GetItems();
//
//    unsigned int index = 0;
//    for (int r = 0; r < rows; r++) {
//        for (int c = 0; c < columns; c++) {
//            if (index >= items.size()) return;
//
//            slotShape.setPosition({
//                70.f + c * 90.f,
//                70.f + r * 90.f
//            });
//            window.draw(slotShape);
//
//            index++;
//        }
//    }
//}
