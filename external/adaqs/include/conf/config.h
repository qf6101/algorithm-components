//
// Created by qfeng on 17-9-25.
//

#ifndef QUICKSCORER_CONFIG_H
#define QUICKSCORER_CONFIG_H

namespace quickscorer {

#define DELTA 4 //search step size

#ifdef ONE_BYTE_BANDWITH
#define BANDWITH_BITS 8
#define BANDWITH_TYPE uint8_t
#elif defined(TWO_BYTES_BANDWITH)
#define BANDWITH_BITS 16
#define BANDWITH_TYPE uint16_t
#elif defined(THREE_BYTES_BANDWITH)
#define BANDWITH_BITS 32
#define BANDWITH_TYPE uint32_t
#elif defined(FOUR_BYTES_BANDWITH)
#define BANDWITH_BITS 64
#define BANDWITH_TYPE uint64_t
#else // set default bandwith bits and type
#define FOUR_BYTES_BANDWITH
#define BANDWITH_BITS 64
#define BANDWITH_TYPE uint64_t
#endif //BANDWITH_BYTES

}

#endif //QUICKSCORER_CONFIG_H
