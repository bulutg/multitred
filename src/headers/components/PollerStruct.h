//
// Created by blt on 6/16/21.
//

#ifndef SIYAH1_POLLERSTRUCT_H
#define SIYAH1_POLLERSTRUCT_H

struct PollerStruct {
    int poll_fd;
    short poll_events;
    sockaddr_in poll_addr;

    bool operator<(const PollerStruct& t) const
    {
        return (this->poll_fd < t.poll_fd);
    }
};

#endif //SIYAH1_PARTNER_H
