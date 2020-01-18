#ifndef MENU_H
#define MENU_H

	#include <vector>
	#include <memory>

	#include "winapi.h"

	class Menu{
		private:
			std::vector<std::shared_ptr<Menu> > subMenus;
			HMENU menu;

		public:
			//constructor
			Menu();
			//destructor
			~Menu();
			//agrega subelementos al menu
			void add(int id,std::string text);
			void add(int id,bool check,std::string text);
			void add(std::shared_ptr<Menu> &subMenu,std::string text);
			//getter y setter
			void setChack(int id,bool check);
			//devuelve el identificador nativo del menu
			HMENU get();
	};

#endif
