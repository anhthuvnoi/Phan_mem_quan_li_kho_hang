#ifndef MODELS_ENTITY_H
#define MODELS_ENTITY_H

#include <string>

// Lớp trừu tượng gốc cho mọi đối tượng có mã + tên
class Entity {
protected:
    std::string id;
    std::string name;

public:
    Entity();
    Entity(const std::string& id, const std::string& name);
    virtual ~Entity();

    std::string getId() const;
    std::string getName() const;
    void setName(const std::string& name);

    virtual std::string toString() const = 0;          // hiển thị (không in trực tiếp)
    virtual std::string toFileString() const = 0;      // ghi file
    virtual void fromFileString(const std::string& line) = 0;
};

#endif // MODELS_ENTITY_H
