#ifndef MODELS_PARTNER_H
#define MODELS_PARTNER_H

#include "models/Entity.h"

// Lớp trừu tượng cho đối tác: Nhà cung cấp, Khách hàng
class Partner : public Entity {
protected:
    std::string phone;
    std::string address;
    std::string email;

public:
    Partner();
    Partner(const std::string& id, const std::string& name,
            const std::string& phone, const std::string& address, const std::string& email);
    virtual ~Partner();

    std::string getPhone() const;
    std::string getAddress() const;
    std::string getEmail() const;
    void setPhone(const std::string& p);
    void setAddress(const std::string& a);
    void setEmail(const std::string& e);
};

#endif // MODELS_PARTNER_H
