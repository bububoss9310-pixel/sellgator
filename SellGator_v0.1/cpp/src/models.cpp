#include "erp/models.hpp"

namespace erp {

double SalesOrder::total_amount() const {
    double total = 0.0;
    for (const auto& line : lines) {
        total += line.unit_price * static_cast<double>(line.quantity);
    }
    return total;
}

}  // namespace erp
