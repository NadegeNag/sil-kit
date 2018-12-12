// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*! 
 * @file MiddlewareTopics.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace { char dummy; }
#endif

#include "MiddlewareTopics.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

ib::mw::sync::idl::QuantumRequest::QuantumRequest()
{

    m_nowNs = 0;

    m_durationNs = 0;


}

ib::mw::sync::idl::QuantumRequest::~QuantumRequest()
{
}

ib::mw::sync::idl::QuantumRequest::QuantumRequest(const QuantumRequest &x)
{
    m_senderAddr = x.m_senderAddr;
    m_nowNs = x.m_nowNs;
    m_durationNs = x.m_durationNs;
}

ib::mw::sync::idl::QuantumRequest::QuantumRequest(QuantumRequest &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_nowNs = x.m_nowNs;
    m_durationNs = x.m_durationNs;
}

ib::mw::sync::idl::QuantumRequest& ib::mw::sync::idl::QuantumRequest::operator=(const QuantumRequest &x)
{
    m_senderAddr = x.m_senderAddr;
    m_nowNs = x.m_nowNs;
    m_durationNs = x.m_durationNs;

    return *this;
}

ib::mw::sync::idl::QuantumRequest& ib::mw::sync::idl::QuantumRequest::operator=(QuantumRequest &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_nowNs = x.m_nowNs;
    m_durationNs = x.m_durationNs;

    return *this;
}

size_t ib::mw::sync::idl::QuantumRequest::getMaxCdrSerializedSize(size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_alignment);
    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);



    return current_alignment - initial_alignment;
}

size_t ib::mw::sync::idl::QuantumRequest::getCdrSerializedSize(const ib::mw::sync::idl::QuantumRequest& data, size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getCdrSerializedSize(data.senderAddr(), current_alignment);
    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);



    return current_alignment - initial_alignment;
}

void ib::mw::sync::idl::QuantumRequest::serialize(eprosima::fastcdr::Cdr &scdr) const
{
    scdr << m_senderAddr;
    scdr << m_nowNs;
    scdr << m_durationNs;
}

void ib::mw::sync::idl::QuantumRequest::deserialize(eprosima::fastcdr::Cdr &dcdr)
{
    dcdr >> m_senderAddr;
    dcdr >> m_nowNs;
    dcdr >> m_durationNs;
}

size_t ib::mw::sync::idl::QuantumRequest::getKeyMaxCdrSerializedSize(size_t current_alignment)
{
	size_t current_align = current_alignment;
            
     current_align += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_align); 



    return current_align;
}

bool ib::mw::sync::idl::QuantumRequest::isKeyDefined()
{
    return true;
}

void ib::mw::sync::idl::QuantumRequest::serializeKey(eprosima::fastcdr::Cdr &scdr) const
{
	(void) scdr;
	 scdr << m_senderAddr;  
	 
	 
}

ib::mw::sync::idl::QuantumGrant::QuantumGrant()
{


    m_nowNs = 0;

    m_durationNs = 0;

    m_status = ib::mw::sync::idl::QR_Invalid;


}

ib::mw::sync::idl::QuantumGrant::~QuantumGrant()
{
}

ib::mw::sync::idl::QuantumGrant::QuantumGrant(const QuantumGrant &x)
{
    m_senderAddr = x.m_senderAddr;
    m_grantee = x.m_grantee;
    m_nowNs = x.m_nowNs;
    m_durationNs = x.m_durationNs;
    m_status = x.m_status;
}

ib::mw::sync::idl::QuantumGrant::QuantumGrant(QuantumGrant &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_grantee = std::move(x.m_grantee);
    m_nowNs = x.m_nowNs;
    m_durationNs = x.m_durationNs;
    m_status = x.m_status;
}

ib::mw::sync::idl::QuantumGrant& ib::mw::sync::idl::QuantumGrant::operator=(const QuantumGrant &x)
{
    m_senderAddr = x.m_senderAddr;
    m_grantee = x.m_grantee;
    m_nowNs = x.m_nowNs;
    m_durationNs = x.m_durationNs;
    m_status = x.m_status;

    return *this;
}

ib::mw::sync::idl::QuantumGrant& ib::mw::sync::idl::QuantumGrant::operator=(QuantumGrant &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_grantee = std::move(x.m_grantee);
    m_nowNs = x.m_nowNs;
    m_durationNs = x.m_durationNs;
    m_status = x.m_status;

    return *this;
}

size_t ib::mw::sync::idl::QuantumGrant::getMaxCdrSerializedSize(size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_alignment);
    current_alignment += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_alignment);
    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

size_t ib::mw::sync::idl::QuantumGrant::getCdrSerializedSize(const ib::mw::sync::idl::QuantumGrant& data, size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getCdrSerializedSize(data.senderAddr(), current_alignment);
    current_alignment += ib::mw::idl::EndpointAddress::getCdrSerializedSize(data.grantee(), current_alignment);
    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

void ib::mw::sync::idl::QuantumGrant::serialize(eprosima::fastcdr::Cdr &scdr) const
{
    scdr << m_senderAddr;
    scdr << m_grantee;
    scdr << m_nowNs;
    scdr << m_durationNs;
    scdr << (uint32_t)m_status;
}

void ib::mw::sync::idl::QuantumGrant::deserialize(eprosima::fastcdr::Cdr &dcdr)
{
    dcdr >> m_senderAddr;
    dcdr >> m_grantee;
    dcdr >> m_nowNs;
    dcdr >> m_durationNs;
    {
        uint32_t enum_value = 0;
        dcdr >> enum_value;
        m_status = (ib::mw::sync::idl::QuantumRequestStatus)enum_value;
    }
}

size_t ib::mw::sync::idl::QuantumGrant::getKeyMaxCdrSerializedSize(size_t current_alignment)
{
	size_t current_align = current_alignment;
            

     current_align += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_align); 




    return current_align;
}

bool ib::mw::sync::idl::QuantumGrant::isKeyDefined()
{
    return true;
}

void ib::mw::sync::idl::QuantumGrant::serializeKey(eprosima::fastcdr::Cdr &scdr) const
{
	(void) scdr;
	 
	 scdr << m_grantee;  
	 
	 
	 
}
ib::mw::sync::idl::Tick::Tick()
{

    m_nowNs = 0;


}

ib::mw::sync::idl::Tick::~Tick()
{
}

ib::mw::sync::idl::Tick::Tick(const Tick &x)
{
    m_senderAddr = x.m_senderAddr;
    m_nowNs = x.m_nowNs;
}

ib::mw::sync::idl::Tick::Tick(Tick &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_nowNs = x.m_nowNs;
}

ib::mw::sync::idl::Tick& ib::mw::sync::idl::Tick::operator=(const Tick &x)
{
    m_senderAddr = x.m_senderAddr;
    m_nowNs = x.m_nowNs;

    return *this;
}

ib::mw::sync::idl::Tick& ib::mw::sync::idl::Tick::operator=(Tick &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_nowNs = x.m_nowNs;

    return *this;
}

size_t ib::mw::sync::idl::Tick::getMaxCdrSerializedSize(size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_alignment);
    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);



    return current_alignment - initial_alignment;
}

size_t ib::mw::sync::idl::Tick::getCdrSerializedSize(const ib::mw::sync::idl::Tick& data, size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getCdrSerializedSize(data.senderAddr(), current_alignment);
    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);



    return current_alignment - initial_alignment;
}

void ib::mw::sync::idl::Tick::serialize(eprosima::fastcdr::Cdr &scdr) const
{
    scdr << m_senderAddr;
    scdr << m_nowNs;
}

void ib::mw::sync::idl::Tick::deserialize(eprosima::fastcdr::Cdr &dcdr)
{
    dcdr >> m_senderAddr;
    dcdr >> m_nowNs;
}

size_t ib::mw::sync::idl::Tick::getKeyMaxCdrSerializedSize(size_t current_alignment)
{
	size_t current_align = current_alignment;
            
     current_align += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_align); 


    return current_align;
}

bool ib::mw::sync::idl::Tick::isKeyDefined()
{
    return true;
}

void ib::mw::sync::idl::Tick::serializeKey(eprosima::fastcdr::Cdr &scdr) const
{
	(void) scdr;
	 scdr << m_senderAddr;  
	 
}
ib::mw::sync::idl::TickDone::TickDone()
{

}

ib::mw::sync::idl::TickDone::~TickDone()
{
}

ib::mw::sync::idl::TickDone::TickDone(const TickDone &x)
{
    m_senderAddr = x.m_senderAddr;
}

ib::mw::sync::idl::TickDone::TickDone(TickDone &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
}

ib::mw::sync::idl::TickDone& ib::mw::sync::idl::TickDone::operator=(const TickDone &x)
{
    m_senderAddr = x.m_senderAddr;

    return *this;
}

ib::mw::sync::idl::TickDone& ib::mw::sync::idl::TickDone::operator=(TickDone &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);

    return *this;
}

size_t ib::mw::sync::idl::TickDone::getMaxCdrSerializedSize(size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_alignment);

    return current_alignment - initial_alignment;
}

size_t ib::mw::sync::idl::TickDone::getCdrSerializedSize(const ib::mw::sync::idl::TickDone& data, size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getCdrSerializedSize(data.senderAddr(), current_alignment);

    return current_alignment - initial_alignment;
}

void ib::mw::sync::idl::TickDone::serialize(eprosima::fastcdr::Cdr &scdr) const
{
    scdr << m_senderAddr;
}

void ib::mw::sync::idl::TickDone::deserialize(eprosima::fastcdr::Cdr &dcdr)
{
    dcdr >> m_senderAddr;
}

size_t ib::mw::sync::idl::TickDone::getKeyMaxCdrSerializedSize(size_t current_alignment)
{
	size_t current_align = current_alignment;
            
     current_align += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_align); 

    return current_align;
}

bool ib::mw::sync::idl::TickDone::isKeyDefined()
{
    return true;
}

void ib::mw::sync::idl::TickDone::serializeKey(eprosima::fastcdr::Cdr &scdr) const
{
	(void) scdr;
	 scdr << m_senderAddr;  
}

ib::mw::sync::idl::ParticipantCommand::ParticipantCommand()
{

    m_participant = 0;

    m_kind = ib::mw::sync::idl::PC_Invalid;


}

ib::mw::sync::idl::ParticipantCommand::~ParticipantCommand()
{
}

ib::mw::sync::idl::ParticipantCommand::ParticipantCommand(const ParticipantCommand &x)
{
    m_senderAddr = x.m_senderAddr;
    m_participant = x.m_participant;
    m_kind = x.m_kind;
}

ib::mw::sync::idl::ParticipantCommand::ParticipantCommand(ParticipantCommand &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_participant = x.m_participant;
    m_kind = x.m_kind;
}

ib::mw::sync::idl::ParticipantCommand& ib::mw::sync::idl::ParticipantCommand::operator=(const ParticipantCommand &x)
{
    m_senderAddr = x.m_senderAddr;
    m_participant = x.m_participant;
    m_kind = x.m_kind;

    return *this;
}

ib::mw::sync::idl::ParticipantCommand& ib::mw::sync::idl::ParticipantCommand::operator=(ParticipantCommand &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_participant = x.m_participant;
    m_kind = x.m_kind;

    return *this;
}

size_t ib::mw::sync::idl::ParticipantCommand::getMaxCdrSerializedSize(size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_alignment);
    current_alignment += 2 + eprosima::fastcdr::Cdr::alignment(current_alignment, 2);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

size_t ib::mw::sync::idl::ParticipantCommand::getCdrSerializedSize(const ib::mw::sync::idl::ParticipantCommand& data, size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getCdrSerializedSize(data.senderAddr(), current_alignment);
    current_alignment += 2 + eprosima::fastcdr::Cdr::alignment(current_alignment, 2);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

void ib::mw::sync::idl::ParticipantCommand::serialize(eprosima::fastcdr::Cdr &scdr) const
{
    scdr << m_senderAddr;
    scdr << m_participant;
    scdr << (uint32_t)m_kind;
}

void ib::mw::sync::idl::ParticipantCommand::deserialize(eprosima::fastcdr::Cdr &dcdr)
{
    dcdr >> m_senderAddr;
    dcdr >> m_participant;
    {
        uint32_t enum_value = 0;
        dcdr >> enum_value;
        m_kind = (ib::mw::sync::idl::ParticipantCommandKind)enum_value;
    }
}

size_t ib::mw::sync::idl::ParticipantCommand::getKeyMaxCdrSerializedSize(size_t current_alignment)
{
	size_t current_align = current_alignment;
            
     current_align += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_align); 



    return current_align;
}

bool ib::mw::sync::idl::ParticipantCommand::isKeyDefined()
{
    return true;
}

void ib::mw::sync::idl::ParticipantCommand::serializeKey(eprosima::fastcdr::Cdr &scdr) const
{
	(void) scdr;
	 scdr << m_senderAddr;  
	 
	 
}

ib::mw::sync::idl::SystemCommand::SystemCommand()
{

    m_kind = ib::mw::sync::idl::SC_Invalid;


}

ib::mw::sync::idl::SystemCommand::~SystemCommand()
{
}

ib::mw::sync::idl::SystemCommand::SystemCommand(const SystemCommand &x)
{
    m_senderAddr = x.m_senderAddr;
    m_kind = x.m_kind;
}

ib::mw::sync::idl::SystemCommand::SystemCommand(SystemCommand &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_kind = x.m_kind;
}

ib::mw::sync::idl::SystemCommand& ib::mw::sync::idl::SystemCommand::operator=(const SystemCommand &x)
{
    m_senderAddr = x.m_senderAddr;
    m_kind = x.m_kind;

    return *this;
}

ib::mw::sync::idl::SystemCommand& ib::mw::sync::idl::SystemCommand::operator=(SystemCommand &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_kind = x.m_kind;

    return *this;
}

size_t ib::mw::sync::idl::SystemCommand::getMaxCdrSerializedSize(size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_alignment);
    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

size_t ib::mw::sync::idl::SystemCommand::getCdrSerializedSize(const ib::mw::sync::idl::SystemCommand& data, size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getCdrSerializedSize(data.senderAddr(), current_alignment);
    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

void ib::mw::sync::idl::SystemCommand::serialize(eprosima::fastcdr::Cdr &scdr) const
{
    scdr << m_senderAddr;
    scdr << (uint32_t)m_kind;
}

void ib::mw::sync::idl::SystemCommand::deserialize(eprosima::fastcdr::Cdr &dcdr)
{
    dcdr >> m_senderAddr;
    {
        uint32_t enum_value = 0;
        dcdr >> enum_value;
        m_kind = (ib::mw::sync::idl::SystemCommandKind)enum_value;
    }
}

size_t ib::mw::sync::idl::SystemCommand::getKeyMaxCdrSerializedSize(size_t current_alignment)
{
	size_t current_align = current_alignment;
            
     current_align += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_align); 


    return current_align;
}

bool ib::mw::sync::idl::SystemCommand::isKeyDefined()
{
    return true;
}

void ib::mw::sync::idl::SystemCommand::serializeKey(eprosima::fastcdr::Cdr &scdr) const
{
	(void) scdr;
	 scdr << m_senderAddr;  
	 
}

ib::mw::sync::idl::ParticipantStatus::ParticipantStatus()
{


    m_state = ib::mw::sync::idl::PS_Invalid;


    m_enterTimeUs = 0;


}

ib::mw::sync::idl::ParticipantStatus::~ParticipantStatus()
{
}

ib::mw::sync::idl::ParticipantStatus::ParticipantStatus(const ParticipantStatus &x)
{
    m_senderAddr = x.m_senderAddr;
    m_participantName = x.m_participantName;
    m_state = x.m_state;
    m_enterReason = x.m_enterReason;
    m_enterTimeUs = x.m_enterTimeUs;
}

ib::mw::sync::idl::ParticipantStatus::ParticipantStatus(ParticipantStatus &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_participantName = std::move(x.m_participantName);
    m_state = x.m_state;
    m_enterReason = std::move(x.m_enterReason);
    m_enterTimeUs = x.m_enterTimeUs;
}

ib::mw::sync::idl::ParticipantStatus& ib::mw::sync::idl::ParticipantStatus::operator=(const ParticipantStatus &x)
{
    m_senderAddr = x.m_senderAddr;
    m_participantName = x.m_participantName;
    m_state = x.m_state;
    m_enterReason = x.m_enterReason;
    m_enterTimeUs = x.m_enterTimeUs;

    return *this;
}

ib::mw::sync::idl::ParticipantStatus& ib::mw::sync::idl::ParticipantStatus::operator=(ParticipantStatus &&x)
{
    m_senderAddr = std::move(x.m_senderAddr);
    m_participantName = std::move(x.m_participantName);
    m_state = x.m_state;
    m_enterReason = std::move(x.m_enterReason);
    m_enterTimeUs = x.m_enterTimeUs;

    return *this;
}

size_t ib::mw::sync::idl::ParticipantStatus::getMaxCdrSerializedSize(size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_alignment);
    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + 255 + 1;

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + 255 + 1;

    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);



    return current_alignment - initial_alignment;
}

size_t ib::mw::sync::idl::ParticipantStatus::getCdrSerializedSize(const ib::mw::sync::idl::ParticipantStatus& data, size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;

    current_alignment += ib::mw::idl::EndpointAddress::getCdrSerializedSize(data.senderAddr(), current_alignment);
    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + data.participantName().size() + 1;

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + data.enterReason().size() + 1;

    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);



    return current_alignment - initial_alignment;
}

void ib::mw::sync::idl::ParticipantStatus::serialize(eprosima::fastcdr::Cdr &scdr) const
{
    scdr << m_senderAddr;
    scdr << m_participantName;
    scdr << (uint32_t)m_state;
    scdr << m_enterReason;
    scdr << m_enterTimeUs;
}

void ib::mw::sync::idl::ParticipantStatus::deserialize(eprosima::fastcdr::Cdr &dcdr)
{
    dcdr >> m_senderAddr;
    dcdr >> m_participantName;
    {
        uint32_t enum_value = 0;
        dcdr >> enum_value;
        m_state = (ib::mw::sync::idl::ParticipantState)enum_value;
    }
    dcdr >> m_enterReason;
    dcdr >> m_enterTimeUs;
}

size_t ib::mw::sync::idl::ParticipantStatus::getKeyMaxCdrSerializedSize(size_t current_alignment)
{
	size_t current_align = current_alignment;
            
     current_align += ib::mw::idl::EndpointAddress::getMaxCdrSerializedSize(current_align); 





    return current_align;
}

bool ib::mw::sync::idl::ParticipantStatus::isKeyDefined()
{
    return true;
}

void ib::mw::sync::idl::ParticipantStatus::serializeKey(eprosima::fastcdr::Cdr &scdr) const
{
	(void) scdr;
	 scdr << m_senderAddr;  
	 
	 
	 
	 
}




