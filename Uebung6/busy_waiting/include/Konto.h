#pragma once
class Konto {

public:
    Konto() = default;
    double getKontoStand();
    void setKontoStand(double kontoStand);

private:
    double kontoStand;
};

