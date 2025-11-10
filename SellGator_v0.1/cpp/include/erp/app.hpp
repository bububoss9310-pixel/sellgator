#pragma once

#include "erp/services.hpp"

#include <memory>

namespace erp {

class Application {
   public:
    Application();
    void run();

   private:
    void show_main_menu() const;
    void handle_add_product();
    void handle_list_products() const;
    void handle_add_customer();
    void handle_list_customers() const;
    void handle_create_order();
    void handle_list_orders() const;

    InventoryService inventory_{};
    CustomerService customers_{};
    std::unique_ptr<SalesService> sales_;
};

}  // namespace erp
