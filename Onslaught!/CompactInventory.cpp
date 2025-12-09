//// InventoryUICompact.cpp
//#include "CompactInventory.hpp"
//#include "InventoryManager.hpp"
//
//InventoryUICompact::InventoryUICompact() {
//    background.setSize({70.f, 300.f});
//    background.setFillColor(sf::Color(50, 50, 50, 200));
//    background.setPosition({10.f, 100.f} );
//
//    slotShape.setSize({180.f, 40.f});
//    slotShape.setFillColor(sf::Color(100, 100, 100));
//    slotShape.setOutlineColor(sf::Color::Black);
//    slotShape.setOutlineThickness(2.f);
//}
//
//void InventoryUICompact::Update() {
//    // no logic needed yet
//}
//
//void InventoryUICompact::Draw(sf::RenderWindow& window) {
//    window.draw(background);
//
//    const auto& items = InventoryManager::GetInstance().GetItems();
//    int count = std::min((int)items.size(), visibleSlots);
//
//    for (int i = 0; i < count; i++) {
//        slotShape.setPosition({20.f, 120.f + i * 45});
//        window.draw(slotShape);
//        // draw item icons/text later
//    }
//}
