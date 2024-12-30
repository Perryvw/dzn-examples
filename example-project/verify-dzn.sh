#!/bin/bash
set -e

echo "Verifying Airlock.dzn"
dzn verify -I dzn-src dzn-src/Airlock.dzn
echo "Verifying AirlockSystem.dzn"
dzn verify -I dzn-src dzn-src/AirlockSystem.dzn
echo "Verifying Armor/AirlockArmor.dzn"
dzn verify -I dzn-src dzn-src/Armor/AirlockArmor.dzn
echo "Verifying Armor/DoorArmor.dzn"
dzn verify -I dzn-src dzn-src/Armor/DoorArmor.dzn
