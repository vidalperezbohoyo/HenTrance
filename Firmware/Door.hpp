#pragma


#pragma once
#include <L298NX2.h>

class Door {
public:
    Door();

    /**
     * @brief Opens the door. Returns true if the operation was successful.
     */
    bool open();

    /**
     * @brief Closes the door. Returns true if the operation was successful.
     */
    bool close();

    /**
     * @brief Indicates if the door is open.
     */
    bool isOpen() const;

    /**
     * @brief Indicates if the door is closed.
     */
    bool isClose() const;

private:
    L298NX2& motors_controller_;
    bool open_ = false;
};