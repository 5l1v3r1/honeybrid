/*
 * This file is part of the honeybrid project.
 *
 * 2007-2009 University of Maryland (http://www.umd.edu)
 * Robin Berthier <robinb@umd.edu>, Thomas Coquelin <coquelin@umd.edu>
 * and Julien Vehent <julien@linuxwall.info>
 *
 * 2012-2014 University of Connecticut (http://www.uconn.edu)
 * Tamas K Lengyel <tamas.k.lengyel@gmail.com>
 *
 * Honeybrid is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/*! \file mod_counter.c
 * \brief Packet counter Module for honeybrid Decision Engine
 *
 * This module returns the position of a packet in the connection
 *
 *
 \author Julien Vehent, 2007
 \author Thomas Coquelin, 2008
 */

#include "modules.h"

/*! mod_counter requires the configuration of the following mandatory parameter:
 - "counter", number of packet to receive before accepting
 */

/*! mod_counter
 \param[in] args, struct that contain the node and the datas to process
 \param[out] set result to 1 packet position match arg, 0 otherwise
 */
mod_result_t mod_counter(struct mod_args *args) {
    uint32_t pktval = 0;
    gchar *param;
    mod_result_t result = DEFER;

    printdbg("%s Module called\n", H(args->pkt->conn->id));

    if ((param = (char *) g_hash_table_lookup(args->node->config, "counter"))
            == NULL) {
        /*! We can't decide */
        printdbg("%s mandatory argument 'counter' undefined!\n",
                H(args->pkt->conn->id));
        return result;
    } else {
        pktval = (uint32_t) atoi(param);
    }

    if (pktval <= args->pkt->conn->count_data_pkt_from_intruder) {
        /*! We accept this packet */
        result = ACCEPT;
        printdbg("%s PACKET MATCH RULE for counter(%d) with value %d\n",
                H(args->pkt->conn->id), pktval,
                args->pkt->conn->count_data_pkt_from_intruder);
    } else {
        /*! We reject this packet */
        result = REJECT;
        printdbg(
                "%s PACKET DOES NOT MATCH RULE for counter(%d) with value %d\n",
                H(args->pkt->conn->id), pktval,
                args->pkt->conn->count_data_pkt_from_intruder);
    }

    return result;
}

