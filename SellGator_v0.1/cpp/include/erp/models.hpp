#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace erp {

struct Product {
    int id{};
    std::string name;
    std::string sku;
    double unit_price{};
    int stock_on_hand{};
};

struct Customer {
    int id{};
    std::string company_name;
    std::string contact_name;
    std::string email;
    std::string phone;
};

struct OrderLine {
    int product_id{};
    int quantity{};
    double unit_price{};
};

struct SalesOrder {
    int id{};
    int customer_id{};
    std::chrono::system_clock::time_point created_at;
    std::vector<OrderLine> lines;

    double total_amount() const;
};

}  // namespace erp
