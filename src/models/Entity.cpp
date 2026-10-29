#include "models/Entity.h"

Entity::Entity() : id(), name() {}
Entity::Entity(const std::string& id, const std::string& name) : id(id), name(name) {}
Entity::~Entity() {}

std::string Entity::getId() const { return id; }
std::string Entity::getName() const { return name; }
void Entity::setName(const std::string& n) { name = n; }
