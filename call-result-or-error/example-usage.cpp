void MethodThatCanFail() {
    // Create error object to fill if something goes wrong
    Dzn::Error error;
    // Call Dezyne generated code and check if succesful or not
    if (m_dezyneSystem.in.MethodThatCanFail(error) != Result::Ok)
    {
        // Throw the error up the stack as an exception
        throw std::runtime_error(std::to_string(error));
    }

    // Otherwise, no error occurred, error is empty here
}