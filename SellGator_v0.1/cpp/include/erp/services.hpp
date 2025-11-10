#pragma once

#include "erp/models.hpp"

#include <optional>
#include <unordered_map>

namespace erp {

struct ProductWithStock {
    Product product;
    int available_stock{};
};

class InventoryService {
   public:
    Product create_product(std::string name, std::string sku, double unit_price, int initial_stock);
    std::optional<Product> find_product(int product_id) const;
    std::vector<ProductWithStock> list_products() const;
    bool adjust_stock(int product_id, int delta);

   private:
    int next_product_id_{1};
    std::unordered_map<int, Product> products_;
};

class CustomerService {
   public:
    Customer create_customer(std::string company_name, std::string contact_name, std::string email,
                             std::string phone);
    std::optional<Customer> find_customer(int customer_id) const;
    std::vector<Customer> list_customers() const;

   private:
    int next_customer_id_{1};
    std::unordered_map<int, Customer> customers_;
};

class SalesService {
   public:
    SalesService(const InventoryService& inventory, const CustomerService& customers);

    std::optional<SalesOrder> create_order(int customer_id, const std::vector<OrderLine>& lines);
    std::vector<SalesOrder> list_orders() const;

   private:
    const InventoryService& inventory_;
    const CustomerService& customers_;
    int next_order_id_{1};
    std::vector<SalesOrder> orders_;
};

}  // namespace erp
