/*********************************************************************************
 *
 * This file is part of ZelosOnline.
 *
 * ZelosOnline is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * ZelosOnline is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright  2013  Rafael Dominguez (npcdoom)
 *
 *********************************************************************************/

#ifndef __ZELOS_LIBSSO_SSO_STALL_H__
#define __ZELOS_LIBSSO_SSO_STALL_H__

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>
#include <boost/thread/mutex.hpp>

#define MAX_STALL_CLIENTS 8
#define MAX_STALL_ITEMS 10

namespace sso
{
    class Item;
    class Storage;

    struct StallItem
    {
        uint8_t pos;
        uint16_t Quantity;
        uint64_t Price;
        boost::shared_ptr<Item> InvItem;
    };

    class Stall : public boost::noncopyable
    {
    public:

        typedef std::map<uint8_t,StallItem>::iterator iterator;

        typedef std::map<uint8_t,StallItem>::const_iterator const_iterator;

    public:

        boost::signals2::signal<void () > signal_insert;

        boost::signals2::signal<void () > signal_remove;

        boost::signals2::signal<void (const std::string&)> signal_greeting;

        boost::signals2::signal<void (uint8_t,uint8_t)> signal_state;

        boost::signals2::signal<void (uint8_t,uint16_t,uint32_t)> signal_price;

        boost::signals2::signal<void (const uint8_t pos,const std::string&)> signal_buy;

        boost::signals2::signal<void (uint32_t,const std::string&)> signal_enter;

        boost::signals2::signal<void (uint32_t)> signal_leave;

        boost::signals2::signal<void (uint32_t)> signal_close;

        boost::signals2::signal<void (const std::string&,const std::string&)> signal_msg;

    public:

        Stall (const std::string& Title, const uint32_t Owner);

        /**
         *
         *	@brief Retrieve stall owner refID.
         *
         *	@retval Owner refID.
         *
         **/

        uint32_t getOwner () const;

        /**
         *
         *	@brief Check if a player is stall owner.
         *
         *	@retval True if its owner, false otherwise.
         *
         **/

        bool IsOwner (const uint32_t playerID) const;

        /**
         *
         *	@brief Set stall title display.
         *
         *	@param Stall title to display.
         *
         **/

        void setTitle (const std::string& title);

        /**
         *
         *	@brief Retrieve stall displayed title.
         *
         *	@retval Stall current title.
         *
         **/

        const std::string& getTitle () const;

        /**
         *
         *	@brief Set stall greeting message.
         *
         *	@param greeting Greeting message to display.
         *
         **/

        void setGreeting (const std::string& greeting);

        /**
         *
         *	@brief Retrieve stall greeting message.
         *
         *	@retval Displayed greeting message.
         *
         **/

        const std::string& getGreeting () const;

        /**
         *
         *	@brief Set stall state and if need to register in stall network.
         *
         *	@param state: Set if the stall is open or not.
         *	@param reg: Set if the stall has been registered in the stall network.
         *
         **/

        void setState (const uint8_t state, const uint8_t reg);

        /**
         *
         *	@brief Retrieve stall open state.
         *
         *	@retval 1 open, 0 closed.
         *
         **/

        uint8_t getState () const;

        /**
         *
         *	@brief Retrieve stall register with stall network state.
         *
         *	@retval 1 registered, 0 not registered.
         *
         **/

        uint8_t getRegister () const;

        /**
         *
         *	@brief Check if the stall is in open state.
         *
         *	@retval True is open, false otherwise.
         *
         **/

        bool IsOpen () const;

        /**
         *
         *	@brief Insert a new client to the stall.
         *
         *	@param playerID: Client refID to add.
         *	@param name: Client name to add.
         *
         **/

        void Enter (const uint32_t playerID, const std::string &name);

        /**
         *
         *	@brief Remove a client from the stall.
         *
         *	@param playerID: Client refID to remove.
         *
         **/

        void Leave (const uint32_t playerID);

        /**
         *
         *	@brief Close stall and remove all clients.
         *
         **/

        void Close ();

        /**
         *
         *	@brief Add a new item to the stall.
         *
         *	@param item: Item description to insert to the stall.
         *
         **/

        void InsertItem (const StallItem &item);

        /**
         *
         *	@brief Modify item stats, the item must be already inside the stall.
         *
         *	@param pos: Item position to modify.
         *	@param quantity: Item quantity to sell.
         *	@param price: Item selling price.
         *
         **/

        void ModifyItem (const uint8_t pos, const uint16_t quantity, const uint64_t price);

        /**
         *
         *	@brief Obtain the item in the selected position.
         *
         *	@param pos Position to search for the item.
         *
         *	@retval If successful a valid item.
         *
         **/

        StallItem Find (const uint8_t pos) const;

        /**
         *
         *	@brief Remove an item from the stall.
         *
         *	@param pos: Position of the item to remove.
         *
         **/

        void RemoveItem (const uint8_t pos);

        /**
         *
         *	@brief Buy an item from the stall.
         *
         *	@param name: Name of the client buyer.
         *	@param pos: Item position to buy.
         *
         **/

        void BuyItem (const std::string &name,const uint8_t pos);

        const_iterator begin () const;

        const_iterator end() const;

        /**
         *
         *	@brief Get stall current client refID list.
         *
         *	@retval refID list.
         *
         **/

        std::vector<uint32_t> getClients () const;

        /**
         *
         *	@brief Check if the stall is full.
         *
         *	@retval True if its full, false otherwise.
         *
         **/

        bool IsFull () const;

        /**
         *
         *	@brief Send a stall message to all the current clients.
         *
         *	@param name: Sender name.
         *	@param msg: Message to deliver.
         *
         **/

        void notifyMsg (const std::string &name, const std::string &msg);

    public:

        mutable boost::mutex m_mutex;

    private:

        std::string m_Name;	//! Stall Name.
        std::string m_Greeting;	//! Stall Greetings.
        uint32_t m_Owner;

        bool m_State;
        bool m_Register;

        std::vector<uint32_t> m_IDs;    //! Players inside the stall
        std::map<uint8_t,StallItem> m_Items;
    };
}

#endif //__ZELOS_LIBSSO_SSO_STALL_H__
