#include <Game/Data/Enemy.hpp>

bool Enemy::operator == (const Enemy &en) const {
    return ID == en.ID;
}

int Enemy::getSingleDamage(int def) {
    // 魔攻的效果
    if (getP(6)) {
        def = 0;
    }

    // 返回单回合伤害
    return std::max(0, std::max(this->atk - def, 0) * std::max(conatk, 1));
}

int Enemy::getDamage(Actor* actor, std::map <int, Element>* elements) {
    // 读入怪物数据
    int ehp = this->hp, ed = getDef(actor);

    // 中毒的效果
    int poison = 100 + motaVariables.variables[5];

    // 吸血的效果
    float vampire = 0;

    // 先攻的效果
    bool first = getP(5);

    // 魔攻的效果
    if (getP(6)) def = 0;

    // 有[v]需要在战斗中使用的话，写在这里
    for (int i = 0, cnt = 1; i < element.size(); ++i) {
        if (elements->at(element[i]).haveBuff) {
            float buff = stof(split(name, "/")[cnt]);
            // 吸血的效果
            if (element[i] == 3)
                vampire = floor(poison / 100.f * getSingleDamage(actor->getDef()) * buff);

            cnt++;
        }
    }

    // 无法破防则返回-1
    if (actor->getAtk() <= ed) return -1;

    // 伤害量够不上吸血量则返回-1
    if (actor->getAtk() - ed <= vampire) return -1;

    // 对方无法破防则返回0
    if (actor->getDef() >= this->atk) return 0;

    // 直接秒杀，仅留下先攻伤害
    if (ehp + vampire * first <= actor->getAtk() - ed) return std::max(0, (this->atk - actor->getDef()) * first);

    // 计算单次伤害，以及回合数
    int edam = actor->getAtk() - ed;
    int eatimes = std::max(0.f, std::ceil((ehp + vampire * first - edam) * 1.f / (edam - vampire)));
    int damage = poison / 100.f * getSingleDamage(actor->getDef()) * eatimes;
    return std::max(damage - actor->mdef, 0);
}

int Enemy::getDef(Actor* actor) const {
    if (getP(4)) return std::max(def, actor->getAtk() - 1);
    return def;
}

bool Enemy::getP(int p) const {
    return vectorFind(element, p);
}

int Enemy::getCrisis(Actor* actor) {
    auto endef = getDef(actor);
    if (actor->getAtk() <= endef) return endef - actor->getAtk() + 1;
    if (actor->getAtk() - endef >= hp) return 0;
    if (getP(4)) return -1;
    return ceil(hp * 1.f / std::max(0.f, std::ceil(hp * 1.f / (actor->getAtk() - endef)) - 1)) + endef - actor->getAtk();
}

std::pair <std::string, std::string> Enemy::getElement(Element element, std::string buff) {
    // 替换buff
    std::string elename = element.name;
    if (getP(7)) // 连击
        elename = std::format("{}{}", conatk,elename);
    std::string desc = element.description;
    if (!buff.empty()) {
        // 重生，替换为变身的怪物名
        if (getP(8)) {
            buff = "另一怪物";
        }
        replaceAll(desc, "[v]", buff);
    }
    return {elename, desc};
}
