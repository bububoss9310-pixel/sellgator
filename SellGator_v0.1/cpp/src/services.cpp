#include "erp/services.hpp"

#include <algorithm>
#include <stdexcept>

namespace erp {

Product InventoryService::create_product(std::string name, std::string sku, double unit_price,
                                         int initial_stock) {
    Product product{next_product_id_++, std::move(name), std::move(sku), unit_price, initial_stock};
    products_.emplace(product.id, product);
    return product;
}

std::optional<Product> InventoryService::find_product(int product_id) const {
    if (auto it = products_.find(product_id); it != products_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<ProductWithStock> InventoryService::list_products() const {
    std::vector<ProductWithStock> items;
    items.reserve(products_.size());
    for (const auto& [id, product] : products_) {
        items.push_back(ProductWithStock{product, product.stock_on_hand});
    }
    std::sort(items.begin(), items.end(), [](const ProductWithStock& lhs, const ProductWithStock& rhs) {
        return lhs.product.id < rhs.product.id;
    });
    return items;
}

bool InventoryService::adjust_stock(int product_id, int delta) {
    auto it = products_.find(product_id);
    if (it == products_.end()) {
        return false;
    }
    auto& product = it->second;
    if (product.stock_on_hand + delta < 0) {
        return false;
    }
    product.stock_on_hand += delta;
    return true;
}

Customer CustomerService::create_customer(std::string company_name, std::string contact_name, std::string email,
                                          std::string phone) {
    Customer customer{next_customer_id_++, std::move(company_name), std::move(contact_name),
                      std::move(email), std::move(phone)};
    customers_.emplace(customer.id, customer);
    return customer;
}

std::optional<Customer> CustomerService::find_customer(int customer_id) const {
    if (auto it = customers_.find(customer_id); it != customers_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<Customer> CustomerService::list_customers() const {
    std::vector<Customer> items;
    items.reserve(customers_.size());
    for (const auto& [id, customer] : customers_) {
        items.push_back(customer);
    }
    std::sort(items.begin(), items.end(), [](const Customer& lhs, const Customer& rhs) {
        return lhs.id < rhs.id;
    });
    return items;
}

SalesService::SalesService(const InventoryService& inventory, const CustomerService& customers)
    : inventory_(inventory), customers_(customers) {}

std::optional<SalesOrder> SalesService::create_order(int customer_id, const std::vector<OrderLine>& lines) {
    if (!customers_.find_customer(customer_id)) {
        return std::nullopt;
    }

    SalesOrder order;
    order.id = next_order_id_++;
    order.customer_id = customer_id;
    order.created_at = std::chrono::system_clock::now();

    for (const auto& line : lines) {
        auto product = inventory_.find_product(line.product_id);
        if (!product || line.quantity <= 0 || line.unit_price < 0.0) {
            return std::nullopt;
        }
        if (product->stock_on_hand < line.quantity) {
            return std::nullopt;
        }
        order.lines.push_back(line);
    }

    orders_.push_back(order);
    return order;
}

std::vector<SalesOrder> SalesService::list_orders() const { return orders_; }

}  // namespace erp
