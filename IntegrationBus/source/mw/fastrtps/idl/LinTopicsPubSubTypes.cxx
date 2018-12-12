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
 * @file LinTopicsPubSubTypes.cpp
 * This header file contains the implementation of the serialization functions.
 *
 * This file was generated by the tool fastcdrgen.
 */


#include <fastcdr/FastBuffer.h>
#include <fastcdr/Cdr.h>

#include "LinTopicsPubSubTypes.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

namespace ib
{
    namespace sim
    {
        namespace lin
        {
            namespace idl
            {




                LinMessagePubSubType::LinMessagePubSubType()
                {
                    setName("ib::sim::lin::idl::LinMessage");
                    m_typeSize = static_cast<uint32_t>(LinMessage::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = LinMessage::isKeyDefined();
                    size_t keyLength = LinMessage::getKeyMaxCdrSerializedSize()>16 ? LinMessage::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                LinMessagePubSubType::~LinMessagePubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool LinMessagePubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    LinMessage *p_type = static_cast<LinMessage*>(data);
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

                bool LinMessagePubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    LinMessage* p_type = static_cast<LinMessage*>(data); //Convert DATA to pointer of your type
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

                std::function<uint32_t()> LinMessagePubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<LinMessage*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* LinMessagePubSubType::createData()
                {
                    return reinterpret_cast<void*>(new LinMessage());
                }

                void LinMessagePubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<LinMessage*>(data));
                }

                bool LinMessagePubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    LinMessage* p_type = static_cast<LinMessage*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),LinMessage::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || LinMessage::getKeyMaxCdrSerializedSize()>16)    {
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

                RxRequestPubSubType::RxRequestPubSubType()
                {
                    setName("ib::sim::lin::idl::RxRequest");
                    m_typeSize = static_cast<uint32_t>(RxRequest::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = RxRequest::isKeyDefined();
                    size_t keyLength = RxRequest::getKeyMaxCdrSerializedSize()>16 ? RxRequest::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                RxRequestPubSubType::~RxRequestPubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool RxRequestPubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    RxRequest *p_type = static_cast<RxRequest*>(data);
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

                bool RxRequestPubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    RxRequest* p_type = static_cast<RxRequest*>(data); //Convert DATA to pointer of your type
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

                std::function<uint32_t()> RxRequestPubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<RxRequest*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* RxRequestPubSubType::createData()
                {
                    return reinterpret_cast<void*>(new RxRequest());
                }

                void RxRequestPubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<RxRequest*>(data));
                }

                bool RxRequestPubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    RxRequest* p_type = static_cast<RxRequest*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),RxRequest::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || RxRequest::getKeyMaxCdrSerializedSize()>16)    {
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

                TxAcknowledgePubSubType::TxAcknowledgePubSubType()
                {
                    setName("ib::sim::lin::idl::TxAcknowledge");
                    m_typeSize = static_cast<uint32_t>(TxAcknowledge::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = TxAcknowledge::isKeyDefined();
                    size_t keyLength = TxAcknowledge::getKeyMaxCdrSerializedSize()>16 ? TxAcknowledge::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                TxAcknowledgePubSubType::~TxAcknowledgePubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool TxAcknowledgePubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    TxAcknowledge *p_type = static_cast<TxAcknowledge*>(data);
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

                bool TxAcknowledgePubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    TxAcknowledge* p_type = static_cast<TxAcknowledge*>(data); //Convert DATA to pointer of your type
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

                std::function<uint32_t()> TxAcknowledgePubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<TxAcknowledge*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* TxAcknowledgePubSubType::createData()
                {
                    return reinterpret_cast<void*>(new TxAcknowledge());
                }

                void TxAcknowledgePubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<TxAcknowledge*>(data));
                }

                bool TxAcknowledgePubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    TxAcknowledge* p_type = static_cast<TxAcknowledge*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),TxAcknowledge::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || TxAcknowledge::getKeyMaxCdrSerializedSize()>16)    {
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


                ControllerConfigPubSubType::ControllerConfigPubSubType()
                {
                    setName("ib::sim::lin::idl::ControllerConfig");
                    m_typeSize = static_cast<uint32_t>(ControllerConfig::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = ControllerConfig::isKeyDefined();
                    size_t keyLength = ControllerConfig::getKeyMaxCdrSerializedSize()>16 ? ControllerConfig::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                ControllerConfigPubSubType::~ControllerConfigPubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool ControllerConfigPubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    ControllerConfig *p_type = static_cast<ControllerConfig*>(data);
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

                bool ControllerConfigPubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    ControllerConfig* p_type = static_cast<ControllerConfig*>(data); //Convert DATA to pointer of your type
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

                std::function<uint32_t()> ControllerConfigPubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<ControllerConfig*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* ControllerConfigPubSubType::createData()
                {
                    return reinterpret_cast<void*>(new ControllerConfig());
                }

                void ControllerConfigPubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<ControllerConfig*>(data));
                }

                bool ControllerConfigPubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    ControllerConfig* p_type = static_cast<ControllerConfig*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),ControllerConfig::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || ControllerConfig::getKeyMaxCdrSerializedSize()>16)    {
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

                SlaveResponseConfigPubSubType::SlaveResponseConfigPubSubType()
                {
                    setName("ib::sim::lin::idl::SlaveResponseConfig");
                    m_typeSize = static_cast<uint32_t>(SlaveResponseConfig::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = SlaveResponseConfig::isKeyDefined();
                    size_t keyLength = SlaveResponseConfig::getKeyMaxCdrSerializedSize()>16 ? SlaveResponseConfig::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                SlaveResponseConfigPubSubType::~SlaveResponseConfigPubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool SlaveResponseConfigPubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    SlaveResponseConfig *p_type = static_cast<SlaveResponseConfig*>(data);
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

                bool SlaveResponseConfigPubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    SlaveResponseConfig* p_type = static_cast<SlaveResponseConfig*>(data); //Convert DATA to pointer of your type
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

                std::function<uint32_t()> SlaveResponseConfigPubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<SlaveResponseConfig*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* SlaveResponseConfigPubSubType::createData()
                {
                    return reinterpret_cast<void*>(new SlaveResponseConfig());
                }

                void SlaveResponseConfigPubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<SlaveResponseConfig*>(data));
                }

                bool SlaveResponseConfigPubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    SlaveResponseConfig* p_type = static_cast<SlaveResponseConfig*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),SlaveResponseConfig::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || SlaveResponseConfig::getKeyMaxCdrSerializedSize()>16)    {
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

                SlaveConfigurationPubSubType::SlaveConfigurationPubSubType()
                {
                    setName("ib::sim::lin::idl::SlaveConfiguration");
                    m_typeSize = static_cast<uint32_t>(SlaveConfiguration::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = SlaveConfiguration::isKeyDefined();
                    size_t keyLength = SlaveConfiguration::getKeyMaxCdrSerializedSize()>16 ? SlaveConfiguration::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                SlaveConfigurationPubSubType::~SlaveConfigurationPubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool SlaveConfigurationPubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    SlaveConfiguration *p_type = static_cast<SlaveConfiguration*>(data);
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

                bool SlaveConfigurationPubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    SlaveConfiguration* p_type = static_cast<SlaveConfiguration*>(data); //Convert DATA to pointer of your type
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

                std::function<uint32_t()> SlaveConfigurationPubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<SlaveConfiguration*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* SlaveConfigurationPubSubType::createData()
                {
                    return reinterpret_cast<void*>(new SlaveConfiguration());
                }

                void SlaveConfigurationPubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<SlaveConfiguration*>(data));
                }

                bool SlaveConfigurationPubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    SlaveConfiguration* p_type = static_cast<SlaveConfiguration*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),SlaveConfiguration::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || SlaveConfiguration::getKeyMaxCdrSerializedSize()>16)    {
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

                SlaveResponsePubSubType::SlaveResponsePubSubType()
                {
                    setName("ib::sim::lin::idl::SlaveResponse");
                    m_typeSize = static_cast<uint32_t>(SlaveResponse::getMaxCdrSerializedSize()) + 4 /*encapsulation*/;
                    m_isGetKeyDefined = SlaveResponse::isKeyDefined();
                    size_t keyLength = SlaveResponse::getKeyMaxCdrSerializedSize()>16 ? SlaveResponse::getKeyMaxCdrSerializedSize() : 16;
                    m_keyBuffer = reinterpret_cast<unsigned char*>(malloc(keyLength));
                    memset(m_keyBuffer, 0, keyLength);
                }

                SlaveResponsePubSubType::~SlaveResponsePubSubType()
                {
                    if(m_keyBuffer!=nullptr)
                        free(m_keyBuffer);
                }

                bool SlaveResponsePubSubType::serialize(void *data, SerializedPayload_t *payload)
                {
                    SlaveResponse *p_type = static_cast<SlaveResponse*>(data);
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

                bool SlaveResponsePubSubType::deserialize(SerializedPayload_t* payload, void* data)
                {
                    SlaveResponse* p_type = static_cast<SlaveResponse*>(data); //Convert DATA to pointer of your type
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

                std::function<uint32_t()> SlaveResponsePubSubType::getSerializedSizeProvider(void* data)
                {
                    return [data]() -> uint32_t
                    {
                        return static_cast<uint32_t>(type::getCdrSerializedSize(*static_cast<SlaveResponse*>(data))) + 4 /*encapsulation*/;
                    };
                }

                void* SlaveResponsePubSubType::createData()
                {
                    return reinterpret_cast<void*>(new SlaveResponse());
                }

                void SlaveResponsePubSubType::deleteData(void* data)
                {
                    delete(reinterpret_cast<SlaveResponse*>(data));
                }

                bool SlaveResponsePubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
                {
                    if(!m_isGetKeyDefined)
                        return false;
                    SlaveResponse* p_type = static_cast<SlaveResponse*>(data);
                    eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(m_keyBuffer),SlaveResponse::getKeyMaxCdrSerializedSize());     // Object that manages the raw buffer.
                    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);     // Object that serializes the data.
                    p_type->serializeKey(ser);
                    if(force_md5 || SlaveResponse::getKeyMaxCdrSerializedSize()>16)    {
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

        } //End of namespace lin

    } //End of namespace sim

} //End of namespace ib
