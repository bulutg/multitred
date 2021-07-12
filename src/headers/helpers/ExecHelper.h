//
// Created by blt on 7/12/21.
//

#ifndef SIYAH1_EXECHELPER_H
#define SIYAH1_EXECHELPER_H

inline std::string execGetOutput(const std::vector<std::string> &args) {
    int p[2];
    pipe(p);

    const pid_t pid = fork();
    if (!pid) {
        close(p[0]);
        dup2(p[1], 1);

        std::vector<char *> vec(args.size() + 1, 0);
        for (size_t i = 0; i < args.size(); ++i) {
            vec[i] = const_cast<char *>(args[i].c_str());
        }

        sleep(5);
        execvp(vec[0], &vec[0]);
        // TODO ERROR
        exit(0);
    }

    close(p[1]);

    std::string out;
    const int buf_size = 4096;
    char buffer[buf_size];
    do {
        const ssize_t r = read(p[0], buffer, buf_size);
        if (r > 0) out.append(buffer, r);
    } while (errno == EAGAIN || errno == EINTR);

    close(p[0]);

    int r, status;
    do {
        r = waitpid(pid, &status, 0);
    } while (r == -1 && errno == EINTR);

    return out;
}


#endif //SIYAH1_EXECHELPER_H
