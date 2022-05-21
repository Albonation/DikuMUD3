#include "file_index_type.h"

#include "textutil.h"
#include "utils.h"
#include "vme.h"

unit_data *file_index_type::find_symbolic_instance_ref(unit_data *ref, ubit16 bitvector)
{
    unit_data *u = nullptr;
    unit_data *uu = nullptr;

    if (ref == nullptr)
    {
        return nullptr;
    }

    if (IS_SET(bitvector, FIND_UNIT_EQUIP))
    {
        for (u = ref->getContainedUnits(); u; u = u->getNext())
        {
            if ((u->getFileIndex() == this) && UNIT_IS_EQUIPPED(u))
            {
                return u;
            }
        }
    }

    if (IS_SET(bitvector, FIND_UNIT_INVEN))
    {
        for (u = ref->getContainedUnits(); u; u = u->getNext())
        {
            if ((u->getFileIndex() == this) && !UNIT_IS_EQUIPPED(u))
            {
                return u;
            }
        }
    }

    if (IS_SET(bitvector, FIND_UNIT_SURRO) && ref->getMyContainer())
    {
        if (this == ref->getMyContainer()->getFileIndex())
        {
            return ref->getMyContainer();
        }

        /* Run through units in local environment */
        for (u = ref->getMyContainer()->getContainedUnits(); u; u = u->getNext())
        {
            if (u->getFileIndex() == this)
            {
                return u;
            }

            /* check tranparancy */
            if (u->getNumberOfCharactersInsideUnit() && UNIT_IS_TRANSPARENT(u))
            {
                for (uu = u->getContainedUnits(); uu; uu = uu->getNext())
                {
                    if (uu->getFileIndex() == this)
                    {
                        return uu;
                    }
                }
            }
        }

        /* Run through units in local environment if upwards transparent */
        if ((u = ref->getMyContainer()->getMyContainer()) && UNIT_IS_TRANSPARENT(ref->getMyContainer()))
        {
            for (u = u->getContainedUnits(); u; u = u->getNext())
            {
                if (u != ref->getMyContainer())
                {
                    if (this == u->getFileIndex())
                    {
                        return u;
                    }

                    /* check down into transparent unit */
                    if (u->getNumberOfCharactersInsideUnit() && UNIT_IS_TRANSPARENT(u))
                    {
                        for (uu = u->getContainedUnits(); uu; uu = uu->getNext())
                        {
                            if (this == uu->getFileIndex())
                            {
                                return uu;
                            }
                        }
                    }
                }
            }
        }
    }

    if (IS_SET(bitvector, FIND_UNIT_ZONE))
    {
        if (!fi_unit_list.empty())
        {
            for (auto it = fi_unit_list.begin(); it != fi_unit_list.end(); it++)
            {
                if ((*it)->getFileIndex() == this)
                {
                    return u;
                }
            }
        }
    }

    if (IS_SET(bitvector, FIND_UNIT_WORLD))
    {
        if (!fi_unit_list.empty())
        {
            return fi_unit_list.front();
        }
    }

    return nullptr;
}

unit_data *file_index_type::find_symbolic_instance()
{
    if (!fi_unit_list.empty())
    {
        return fi_unit_list.front();
    }

    return nullptr;
}

const char *file_index_type::getName() const
{
    return name.c_str();
}

long file_index_type::getFilepos() const
{
    return filepos;
}

ubit32 file_index_type::getCRC() const
{
    return crc;
}

sbit16 file_index_type::getNumInZone() const
{
    return no_in_zone;
}

ubit16 file_index_type::getRoomNum() const
{
    return room_no;
}

ubit8 file_index_type::getType() const
{
    return type;
}

ubit16 file_index_type::getNumInMem() const
{
    return no_in_mem;
}

ubit32 file_index_type::getLength() const
{
    return length;
}

void file_index_type::IncrementNumInMemory()
{
    ++no_in_mem;
}

zone_type *file_index_type::getZone() const
{
    return zone;
}

void file_index_type::setType(ubit8 value)
{
    type = value;
}

void file_index_type::setZone(zone_type *value)
{
    zone = value;
}

void file_index_type::setName(const char *value, bool to_lower)
{
    if (value)
    {
        name = std::string(value);
        if (to_lower)
        {
            str_lower(name);
        }
    }
}

void file_index_type::DecrementNumInMemory()
{
    /// @todo find out why code is trying to decrement no_in_mem below 0
    ///   I tried this but somewhere it goes below zero
    ///     assert(no_in_mem > 0);
    if (no_in_mem > 0)
    {
        --no_in_mem;
    }
}

void file_index_type::IncrementNumInZone()
{
    ++no_in_zone;
}

void file_index_type::setNumInZone(sbit16 value)
{
    no_in_zone = value;
}

void file_index_type::setLength(ubit32 value)
{
    length = value;
}

void file_index_type::setFilepos(long value)
{
    filepos = value;
}

void file_index_type::setRoomNum(ubit16 value)
{
    room_no = value;
}

void file_index_type::setNumInMemory(sbit16 value)
{
    no_in_mem = value;
}

void file_index_type::setCRC(ubit32 value)
{
    crc = value;
}

bool file_index_type::Empty() const
{
    return fi_unit_list.empty();
}

unit_data *file_index_type::Front() const
{
    return fi_unit_list.front();
}

std::forward_list<unit_data *>::iterator file_index_type::Begin()
{
    return fi_unit_list.begin();
}

std::forward_list<unit_data *>::iterator file_index_type::End()
{
    return fi_unit_list.end();
}

void file_index_type::PushFront(unit_data *value)
{
    fi_unit_list.push_front(value);
}

void file_index_type::Remove(unit_data *value)
{
    fi_unit_list.remove(value);
}
