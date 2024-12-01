#include "Card.h"

void Card::update() {
	if (isSelected) {
		switch (this->type) {
		case CardType::ONE:
			this->setShape("****** # ** # ** # ** # ** # ******");
			break;
		case CardType::TWO:
			this->setShape("******###**  #**###**#  **###******");
			break;
		case CardType::THREE:
			this->setShape("******###**  #**###**  #**###******");
			break;
		case CardType::FOUR:
			this->setShape("******# #**# #**###**  #**  #******");
			break;
		case CardType::FIVE:
			this->setShape("******###**#  **###**  #**###******");
			break;
		}
	}
	else {
		switch (this->type) {
		case CardType::ONE:
			this->setShape("       #    #    #    #    #       ");
			break;
		case CardType::TWO:
			this->setShape("      ###    #  ###  #    ###      ");
			break;
		case CardType::THREE:
			this->setShape("      ###    #  ###    #  ###      ");
			break;
		case CardType::FOUR:
			this->setShape("      # #  # #  ###    #    #      ");
			break;
		case CardType::FIVE:
			this->setShape("      ###  #    ###    #  ###      ");
			break;
		}
	}
	
}