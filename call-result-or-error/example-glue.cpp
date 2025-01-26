void GlueImpl::ConnectWith(DznInterface& port)
{
    port.in.Open = [this](Dzn::Error& error) noexcept {
        try // Event handlers must NEVER throw
        {
            Open();
        } 
        catch (std::exception& e)
        {
            // Add exception message to error
            error.AddMessage(e.what());
            // Return Error result
            ::Result::Error;
        }
        // Return Ok result
        return ::Result::Ok;
    };
}