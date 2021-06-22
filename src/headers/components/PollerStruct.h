//
// Created by blt on 6/16/21.
//

#ifndef SIYAH1_PARTNER_H
#define SIYAH1_PARTNER_H

struct PollerStruct {
    int poll_fd;
    short poll_events;
    short poll_revents;

    bool operator<(const PollerStruct& t) const
    {
        return (this->poll_fd < t.poll_fd);
    }
};

#endif //SIYAH1_PARTNER_H
