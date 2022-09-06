//
// Created by denis on 02.08.2022.
//

#ifndef COURSEWORKDB_IWINDOW_H
#define COURSEWORKDB_IWINDOW_H

namespace polytour::ui {
    class IWindow {
    public:

        virtual ~IWindow() = default;

        virtual void destroy() = 0;

        virtual void init() = 0;

    };
}

#endif //COURSEWORKDB_IWINDOW_H
