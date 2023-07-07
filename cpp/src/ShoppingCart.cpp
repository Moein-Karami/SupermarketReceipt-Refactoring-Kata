#include "ShoppingCart.h"

void addItemQuantity(const Product& product, double quantity);

std::vector<ProductQuantity> ShoppingCart::getItems() const {
    return items;
}

std::map<Product, double> ShoppingCart::getProductQuantities() const {
    return productQuantities;
}

void ShoppingCart::addItem(const Product& product) {
    addItemQuantity(product, 1.0);
}

void ShoppingCart::addItemQuantity(const Product& product, double quantity) {
    items.emplace_back(product, quantity);
    if (productQuantities.find(product) != productQuantities.end()) {
        productQuantities[product] += quantity;
    } else {
        productQuantities[product] = quantity;
    }
}

void ShoppingCart::handleOffers(Receipt& receipt, std::map<Product, Offer> offers, SupermarketCatalog* catalog) {
    for (const auto& productQuantity : productQuantities) {
        Product product = productQuantity.first;
        double quantity = productQuantity.second;
        if (offers.find(product) != offers.end()) {
            auto offer = offers[product];
            double unitPrice = catalog->getUnitPrice(product);
            int quantityAsInt = (int) quantity;
            Discount* discount = nullptr;
            double discountAmount = calculateDiscountAmount(offer, quantity, unitPrice);

            if (offer.getOfferType() == SpecialOfferType::TwoForAmount && quantityAsInt >= 2) {
                discount = new Discount("2 for " + std::to_string(offer.getArgument()), discountAmount, product);
            }
            else if (offer.getOfferType() == SpecialOfferType::ThreeForTwo && quantityAsInt > 2) {
                discount = new Discount("3 for 2", discountAmount, product);
            }
            else if (offer.getOfferType() == SpecialOfferType::TenPercentDiscount) {
                discount = new Discount(std::to_string(offer.getArgument()) + "% off", discountAmount, product);
            }
            else if (offer.getOfferType() == SpecialOfferType::FiveForAmount && quantityAsInt >= 5) {
                discount = new Discount(std::to_string(5) + " for " + std::to_string(offer.getArgument()),
                discountAmount, product);
            }

            if (discount != nullptr)
                receipt.addDiscount(*discount);
        }
    }
}

double ShoppingCart::calculateDiscountAmount(Offer offer, double quantity, double unitPrice)
{
    int quantityAsInt = int (quantity);
    if (offer.getOfferType() == SpecialOfferType::TwoForAmount) {
        return -(unitPrice * quantity - (offer.getArgument() * (quantityAsInt / 2) + quantityAsInt % 2 * unitPrice));
    }
    else if (offer.getOfferType() == SpecialOfferType::ThreeForTwo) {
        return -(quantity * unitPrice - ((quantityAsInt / 3 * 2 * unitPrice) + quantityAsInt % 3
        * unitPrice));
    }
    else if (offer.getOfferType() == SpecialOfferType::TenPercentDiscount) {
        return -quantity * unitPrice * offer.getArgument() / 100.0;
    }
    else if (offer.getOfferType() == SpecialOfferType::FiveForAmount) {
        return -(unitPrice * quantity - (offer.getArgument() * quantityAsInt / 5 + quantityAsInt 
        % 5 * unitPrice));
    }
    return 0;
}