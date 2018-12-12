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
 * @file CanTopicsPubSubTypes.cpp
 * This header file contains the implementation of the serialization functions.
 *
 * This file was generated by the tool fastcdrgen.
 */


#include <fastcdr/FastBuffer.h>
#include <fastcdr/Cdr.h>

#include "CanTopicsPubSubTypes.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

namespace ib
{
    namespace sim
    {
        namespace can
        {
            namespace idl
            {
                CanMessageFlagsPubSubType::CanMessageFlagsPubSubType()
                {
                    setName("ib::sim::can::idl::CanMessageFlags");
                    m_typeSize = static_cast<uint32_t>(CanMessageFlags::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = CanMessageFlags::isKeyDefined();
                    size_t keyLength = CanMessageFlags::getKeyMaxCdrSerializedSize()>16 ? CanMessageFlags::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                CanMessageFlagsPubSubType::~CanMessageFlagsPubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool CanMessageFlagsPubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    CanMessageFlags *p_type = static_cast<CanMessageFlags*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->max_size); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
                    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
                    // Serialize encapsulation
                    ser.serialize_encapsulation();

                    try
                    {
                        p_type->serialize(ser); // Serialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    payload->length = static_cast<uint32_t>(ser.getSerializedDataLength()); //Get the serialized length
                    return true;
                }

                bool CanMessageFlagsPubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    CanMessageFlags* p_type = static_cast<CanMessageFlags*>(data); //Convert DATA to pointer of your type
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->length); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.
                    // Deserialize encapsulation.
                    deser.read_encapsulation();
                    payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

                    try
                    {
                        p_type->deserialize(deser); //Deserialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    return true;
                }

                std::function<uint32_t()> CanMessageFlagsPubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<CanMessageFlags*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* CanMessageFlagsPubSubType::createData()
                {
                    return reinterpret_cast<void*>(new CanMessageFlags());
                }

                void CanMessageFlagsPubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<CanMessageFlags*>(data));
                }

                bool CanMessageFlagsPubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    CanMessageFlags* p_type = static_cast<CanMessageFlags*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),CanMessageFlags::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || CanMessageFlags::getKeyMaxCdrSerializedSize()>16)    {
                        m_md5.init();
                        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.getSerializedDataLength()));
                        m_md5.finalize();
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_md5.digest[i];
                        }
                    }
                    else    {
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_keyBuffer[i];
                        }
                    }
                    return true;
                }


                CanMessagePubSubType::CanMessagePubSubType()
                {
                    setName("ib::sim::can::idl::CanMessage");
                    m_typeSize = static_cast<uint32_t>(CanMessage::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = CanMessage::isKeyDefined();
                    size_t keyLength = CanMessage::getKeyMaxCdrSerializedSize()>16 ? CanMessage::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                CanMessagePubSubType::~CanMessagePubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool CanMessagePubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    CanMessage *p_type = static_cast<CanMessage*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->max_size); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
                    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
                    // Serialize encapsulation
                    ser.serialize_encapsulation();

                    try
                    {
                        p_type->serialize(ser); // Serialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    payload->length = static_cast<uint32_t>(ser.getSerializedDataLength()); //Get the serialized length
                    return true;
                }

                bool CanMessagePubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    CanMessage* p_type = static_cast<CanMessage*>(data); //Convert DATA to pointer of your type
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->length); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.
                    // Deserialize encapsulation.
                    deser.read_encapsulation();
                    payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

                    try
                    {
                        p_type->deserialize(deser); //Deserialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    return true;
                }

                std::function<uint32_t()> CanMessagePubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<CanMessage*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* CanMessagePubSubType::createData()
                {
                    return reinterpret_cast<void*>(new CanMessage());
                }

                void CanMessagePubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<CanMessage*>(data));
                }

                bool CanMessagePubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    CanMessage* p_type = static_cast<CanMessage*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),CanMessage::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || CanMessage::getKeyMaxCdrSerializedSize()>16)    {
                        m_md5.init();
                        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.getSerializedDataLength()));
                        m_md5.finalize();
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_md5.digest[i];
                        }
                    }
                    else    {
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_keyBuffer[i];
                        }
                    }
                    return true;
                }

                CanControllerStatusPubSubType::CanControllerStatusPubSubType()
                {
                    setName("ib::sim::can::idl::CanControllerStatus");
                    m_typeSize = static_cast<uint32_t>(CanControllerStatus::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = CanControllerStatus::isKeyDefined();
                    size_t keyLength = CanControllerStatus::getKeyMaxCdrSerializedSize()>16 ? CanControllerStatus::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                CanControllerStatusPubSubType::~CanControllerStatusPubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool CanControllerStatusPubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    CanControllerStatus *p_type = static_cast<CanControllerStatus*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->max_size); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
                    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
                    // Serialize encapsulation
                    ser.serialize_encapsulation();

                    try
                    {
                        p_type->serialize(ser); // Serialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    payload->length = static_cast<uint32_t>(ser.getSerializedDataLength()); //Get the serialized length
                    return true;
                }

                bool CanControllerStatusPubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    CanControllerStatus* p_type = static_cast<CanControllerStatus*>(data); //Convert DATA to pointer of your type
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->length); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.
                    // Deserialize encapsulation.
                    deser.read_encapsulation();
                    payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

                    try
                    {
                        p_type->deserialize(deser); //Deserialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    return true;
                }

                std::function<uint32_t()> CanControllerStatusPubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<CanControllerStatus*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* CanControllerStatusPubSubType::createData()
                {
                    return reinterpret_cast<void*>(new CanControllerStatus());
                }

                void CanControllerStatusPubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<CanControllerStatus*>(data));
                }

                bool CanControllerStatusPubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    CanControllerStatus* p_type = static_cast<CanControllerStatus*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),CanControllerStatus::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || CanControllerStatus::getKeyMaxCdrSerializedSize()>16)    {
                        m_md5.init();
                        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.getSerializedDataLength()));
                        m_md5.finalize();
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_md5.digest[i];
                        }
                    }
                    else    {
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_keyBuffer[i];
                        }
                    }
                    return true;
                }

                CanTransmitAcknowledgePubSubType::CanTransmitAcknowledgePubSubType()
                {
                    setName("ib::sim::can::idl::CanTransmitAcknowledge");
                    m_typeSize = static_cast<uint32_t>(CanTransmitAcknowledge::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = CanTransmitAcknowledge::isKeyDefined();
                    size_t keyLength = CanTransmitAcknowledge::getKeyMaxCdrSerializedSize()>16 ? CanTransmitAcknowledge::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                CanTransmitAcknowledgePubSubType::~CanTransmitAcknowledgePubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool CanTransmitAcknowledgePubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    CanTransmitAcknowledge *p_type = static_cast<CanTransmitAcknowledge*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->max_size); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
                    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
                    // Serialize encapsulation
                    ser.serialize_encapsulation();

                    try
                    {
                        p_type->serialize(ser); // Serialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    payload->length = static_cast<uint32_t>(ser.getSerializedDataLength()); //Get the serialized length
                    return true;
                }

                bool CanTransmitAcknowledgePubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    CanTransmitAcknowledge* p_type = static_cast<CanTransmitAcknowledge*>(data); //Convert DATA to pointer of your type
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->length); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.
                    // Deserialize encapsulation.
                    deser.read_encapsulation();
                    payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

                    try
                    {
                        p_type->deserialize(deser); //Deserialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    return true;
                }

                std::function<uint32_t()> CanTransmitAcknowledgePubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<CanTransmitAcknowledge*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* CanTransmitAcknowledgePubSubType::createData()
                {
                    return reinterpret_cast<void*>(new CanTransmitAcknowledge());
                }

                void CanTransmitAcknowledgePubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<CanTransmitAcknowledge*>(data));
                }

                bool CanTransmitAcknowledgePubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    CanTransmitAcknowledge* p_type = static_cast<CanTransmitAcknowledge*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),CanTransmitAcknowledge::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || CanTransmitAcknowledge::getKeyMaxCdrSerializedSize()>16)    {
                        m_md5.init();
                        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.getSerializedDataLength()));
                        m_md5.finalize();
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_md5.digest[i];
                        }
                    }
                    else    {
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_keyBuffer[i];
                        }
                    }
                    return true;
                }

                CanConfigureBaudratePubSubType::CanConfigureBaudratePubSubType()
                {
                    setName("ib::sim::can::idl::CanConfigureBaudrate");
                    m_typeSize = static_cast<uint32_t>(CanConfigureBaudrate::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = CanConfigureBaudrate::isKeyDefined();
                    size_t keyLength = CanConfigureBaudrate::getKeyMaxCdrSerializedSize()>16 ? CanConfigureBaudrate::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                CanConfigureBaudratePubSubType::~CanConfigureBaudratePubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool CanConfigureBaudratePubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    CanConfigureBaudrate *p_type = static_cast<CanConfigureBaudrate*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->max_size); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
                    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
                    // Serialize encapsulation
                    ser.serialize_encapsulation();

                    try
                    {
                        p_type->serialize(ser); // Serialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    payload->length = static_cast<uint32_t>(ser.getSerializedDataLength()); //Get the serialized length
                    return true;
                }

                bool CanConfigureBaudratePubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    CanConfigureBaudrate* p_type = static_cast<CanConfigureBaudrate*>(data); //Convert DATA to pointer of your type
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->length); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.
                    // Deserialize encapsulation.
                    deser.read_encapsulation();
                    payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

                    try
                    {
                        p_type->deserialize(deser); //Deserialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    return true;
                }

                std::function<uint32_t()> CanConfigureBaudratePubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<CanConfigureBaudrate*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* CanConfigureBaudratePubSubType::createData()
                {
                    return reinterpret_cast<void*>(new CanConfigureBaudrate());
                }

                void CanConfigureBaudratePubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<CanConfigureBaudrate*>(data));
                }

                bool CanConfigureBaudratePubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    CanConfigureBaudrate* p_type = static_cast<CanConfigureBaudrate*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),CanConfigureBaudrate::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || CanConfigureBaudrate::getKeyMaxCdrSerializedSize()>16)    {
                        m_md5.init();
                        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.getSerializedDataLength()));
                        m_md5.finalize();
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_md5.digest[i];
                        }
                    }
                    else    {
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_keyBuffer[i];
                        }
                    }
                    return true;
                }

                CanSetControllerModePubSubType::CanSetControllerModePubSubType()
                {
                    setName("ib::sim::can::idl::CanSetControllerMode");
                    m_typeSize = static_cast<uint32_t>(CanSetControllerMode::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = CanSetControllerMode::isKeyDefined();
                    size_t keyLength = CanSetControllerMode::getKeyMaxCdrSerializedSize()>16 ? CanSetControllerMode::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                CanSetControllerModePubSubType::~CanSetControllerModePubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool CanSetControllerModePubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    CanSetControllerMode *p_type = static_cast<CanSetControllerMode*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->max_size); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
                    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
                    // Serialize encapsulation
                    ser.serialize_encapsulation();

                    try
                    {
                        p_type->serialize(ser); // Serialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    payload->length = static_cast<uint32_t>(ser.getSerializedDataLength()); //Get the serialized length
                    return true;
                }

                bool CanSetControllerModePubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    CanSetControllerMode* p_type = static_cast<CanSetControllerMode*>(data); //Convert DATA to pointer of your type
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload->data), payload->length); // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.
                    // Deserialize encapsulation.
                    deser.read_encapsulation();
                    payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

                    try
                    {
                        p_type->deserialize(deser); //Deserialize the object:
                    }
                    catch(eprosima::fastcdr::exception::NotEnoughMemoryException& /*exception*/)
                    {
                        return false;
                    }

                    return true;
                }

                std::function<uint32_t()> CanSetControllerModePubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<CanSetControllerMode*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* CanSetControllerModePubSubType::createData()
                {
                    return reinterpret_cast<void*>(new CanSetControllerMode());
                }

                void CanSetControllerModePubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<CanSetControllerMode*>(data));
                }

                bool CanSetControllerModePubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    CanSetControllerMode* p_type = static_cast<CanSetControllerMode*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),CanSetControllerMode::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || CanSetControllerMode::getKeyMaxCdrSerializedSize()>16)    {
                        m_md5.init();
                        m_md5.update(m_keyBuffer, static_cast<unsigned int>(ser.getSerializedDataLength()));
                        m_md5.finalize();
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_md5.digest[i];
                        }
                    }
                    else    {
                        for(uint8_t i = 0;i<16;++i)        {
                            handle->value[i] = m_keyBuffer[i];
                        }
                    }
                    return true;
                }


            } //End of namespace idl

        } //End of namespace can

    } //End of namespace sim

} //End of namespace ib
