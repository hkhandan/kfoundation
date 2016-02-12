/*---[MasterMemoryManager.h]-----------------------------------m(._.)m--------*\
 |
 |  Project   : KFoundation
 |  Declares  : MasterMemoryManager::*
 |  Implements: -
 |
 |  Copyright (c) 2013, 2014, 2015, RIKEN (The Institute of Physical and
 |  Chemial Research) All rights reserved.
 |
 |  Author: Hamed KHANDAN (hamed.khandan@port.kobe-u.ac.jp)
 |
 |  This file is distributed under the KnoRBA Free Public License. See
 |  LICENSE.TXT for details.
 |
 *//////////////////////////////////////////////////////////////////////////////

#ifndef KFOUNDATION_MASTERMEMORYMANAGER
#define KFOUNDATION_MASTERMEMORYMANAGER

#include "definitions.h"
#include "MemoryManager.h"

namespace kfoundation {
  
  class KFObject;

  /**
   * Manages all the memory managers used in a process.
   * Only once instance of this class exists per process.
   * This instance can be obtained via System::getMasterMemoryManager().
   * 
   * This object is initialized with two memory managers. (1) The manager
   * obtained by getDefaultManager() is the one responsible for ref-counted
   * garbage collection. (2) The manager obtained by getStaticManager() is
   * responsible for objects with unlimited lifetime.
   *
   * @ingroup memory
   * @headerfile MasterMemoryManager.h <kfoundation/MasterMemoryManager.h>
   */
  
  class MasterMemoryManager {
    
  // --- FIELDS --- //
    
    private: MemoryManager::ObjectRecord**  _recordTable;
    private: MemoryManager** _managers;
    private: MemoryManager*  _staticManager;
    private: MemoryManager*  _defaultManager;
    private: kf_int8_t _nManagers;

  
  // --- (DE)CONSTRUCTORS --- //
    
    public: MasterMemoryManager();
    public: ~MasterMemoryManager();

    
  // --- METHODS --- //

    public: inline KFObject* refToPtr(kf_uref_t ref);
    public: inline MemoryManager& getDefaultManager() const;
    public: inline MemoryManager& getStaticManager() const;
    public: inline MemoryManager::ObjectRecord** getMasterTable();
    public: int addManager(MemoryManager* manager, MemoryManager::ObjectRecord* table, kf_int32_t size);
    public: void updateManager(int index, MemoryManager::ObjectRecord* table, kf_int32_t size);
    public: void removeManager(int index);
    public: kf_int8_t getManagerCount() const;
    public: MemoryManager& getManagerAtIndex(int index) const;
    public: void migrate(MasterMemoryManager& other);
    public: void printStats() const;

  };


  /**
   * Deferences the given parametes into the corresponding memory address, 
   * if valid.
   */

  inline KFObject* MasterMemoryManager::refToPtr(kf_uref_t ref) {
    return getManagerAtIndex(ref.manager).getObject(ref.index, ref.key);
  }

  /**
   * Returns the default reference-conting memory manager.
   */

  inline MemoryManager& MasterMemoryManager::getDefaultManager() const {
    return *_defaultManager;
  }


  /**
   * Returns the default static memory manager.
   */

  inline MemoryManager& MasterMemoryManager::getStaticManager() const {
    return *_staticManager;
  }


  inline MemoryManager::ObjectRecord** MasterMemoryManager::getMasterTable() {
    return _recordTable;
  }

} // namespace kfoundation

#endif /* defined(KFOUNDATION_MASTERMEMORYMANAGER) */