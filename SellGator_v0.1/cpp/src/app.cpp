#include "erp/app.hpp"

#include <iomanip>
#include <iostream>
#include <limits>

namespace erp {

namespace {
int read_int(const std::string& prompt) {
    std::cout << prompt;
    int value = 0;
    std::cin >> value;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

double read_double(const std::string& prompt) {
    std::cout << prompt;
    double value = 0.0;
    std::cin >> value;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string read_line(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}
}  // namespace

Application::Application() { sales_ = std::make_unique<SalesService>(inventory_, customers_); }

void Application::run() {
    bool running = true;
    while (running) {
        show_main_menu();
        std::cout << "Choisissez une option: ";
        int option = 0;
        std::cin >> option;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (option) {
            case 1:
                handle_add_product();
                break;
            case 2:
                handle_list_products();
                break;
            case 3:
                handle_add_customer();
                break;
            case 4:
                handle_list_customers();
                break;
            case 5:
                handle_create_order();
                break;
            case 6:
                handle_list_orders();
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Option invalide, veuillez réessayer.\n";
                break;
        }
    }
}

void Application::show_main_menu() const {
    std::cout << "\n========== SellGator ERP ==========" << std::endl;
    std::cout << "1. Ajouter un produit" << std::endl;
    std::cout << "2. Lister les produits" << std::endl;
    std::cout << "3. Ajouter un client" << std::endl;
    std::cout << "4. Lister les clients" << std::endl;
    std::cout << "5. Créer une commande" << std::endl;
    std::cout << "6. Lister les commandes" << std::endl;
    std::cout << "0. Quitter" << std::endl;
}

void Application::handle_add_product() {
    std::string name = read_line("Nom du produit: ");
    std::string sku = read_line("SKU: ");
    double price = read_double("Prix unitaire (€): ");
    int stock = read_int("Stock initial: ");

    Product product = inventory_.create_product(name, sku, price, stock);
    std::cout << "Produit créé avec l'ID " << product.id << "\n";
}

void Application::handle_list_products() const {
    auto products = inventory_.list_products();
    if (products.empty()) {
        std::cout << "Aucun produit enregistré.\n";
        return;
    }

    std::cout << std::left << std::setw(6) << "ID" << std::setw(20) << "Nom" << std::setw(12) << "SKU" << std::setw(10)
              << "Prix" << std::setw(10) << "Stock" << '\n';
    for (const auto& item : products) {
        std::cout << std::left << std::setw(6) << item.product.id << std::setw(20) << item.product.name << std::setw(12)
                  << item.product.sku << std::setw(10) << item.product.unit_price << std::setw(10)
                  << item.available_stock << '\n';
    }
}

void Application::handle_add_customer() {
    std::string company = read_line("Nom de l'entreprise: ");
    std::string contact = read_line("Nom du contact: ");
    std::string email = read_line("Email: ");
    std::string phone = read_line("Téléphone: ");

    Customer customer = customers_.create_customer(company, contact, email, phone);
    std::cout << "Client enregistré avec l'ID " << customer.id << "\n";
}

void Application::handle_list_customers() const {
    auto customers = customers_.list_customers();
    if (customers.empty()) {
        std::cout << "Aucun client enregistré.\n";
        return;
    }

    std::cout << std::left << std::setw(6) << "ID" << std::setw(20) << "Société" << std::setw(20) << "Contact"
              << std::setw(25) << "Email" << std::setw(15) << "Téléphone" << '\n';
    for (const auto& customer : customers) {
        std::cout << std::left << std::setw(6) << customer.id << std::setw(20) << customer.company_name << std::setw(20)
                  << customer.contact_name << std::setw(25) << customer.email << std::setw(15) << customer.phone
                  << '\n';
    }
}

void Application::handle_create_order() {
    int customer_id = read_int("ID du client: ");
    auto customer = customers_.find_customer(customer_id);
    if (!customer) {
        std::cout << "Client introuvable.\n";
        return;
    }

    std::vector<OrderLine> lines;
    while (true) {
        int product_id = read_int("ID du produit (0 pour terminer): ");
        if (product_id == 0) {
            break;
        }
        auto product = inventory_.find_product(product_id);
        if (!product) {
            std::cout << "Produit introuvable.\n";
            continue;
        }
        int quantity = read_int("Quantité: ");
        if (quantity <= 0) {
            std::cout << "Quantité invalide.\n";
            continue;
        }
        if (product->stock_on_hand < quantity) {
            std::cout << "Stock insuffisant (disponible: " << product->stock_on_hand << ").\n";
            continue;
        }
        lines.push_back(OrderLine{product_id, quantity, product->unit_price});
        inventory_.adjust_stock(product_id, -quantity);
        std::cout << "Article ajouté à la commande.\n";
    }

    if (lines.empty()) {
        std::cout << "Aucun article ajouté, commande annulée.\n";
        return;
    }

    auto order = sales_->create_order(customer_id, lines);
    if (!order) {
        std::cout << "Impossible de créer la commande.\n";
        return;
    }

    std::cout << "Commande créée avec l'ID " << order->id << ", montant total: " << order->total_amount()
              << "€\n";
}

void Application::handle_list_orders() const {
    auto orders = sales_->list_orders();
    if (orders.empty()) {
        std::cout << "Aucune commande enregistrée.\n";
        return;
    }

    for (const auto& order : orders) {
        std::cout << "Commande " << order.id << " pour le client " << order.customer_id << " - Total: "
                  << order.total_amount() << "€\n";
        for (const auto& line : order.lines) {
            std::cout << "  Produit " << line.product_id << " x" << line.quantity << " @ " << line.unit_price
                      << "€\n";
        }
    }
}

}  // namespace erp
