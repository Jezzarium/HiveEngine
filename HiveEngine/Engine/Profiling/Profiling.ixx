export module Profiling;

import Core;
import std;
namespace hive
{
    class Tracy
    {
    public:
        void Alloc(unsigned int size, std::source_location loc, void *ptr)
        {
        }

        void Free(void *ptr)
        {
        }
    };

    export class ProfilingModule : public Module<ProfilingModule>
    {
    public:

        const char *GetName() const override { return "ProfilingModule"; }

    protected:
        void DoConfigure(ModuleContext &context) override{}

        void DoInitialize() override{}

        void DoShutdown() override{}
    private:
        Tracy m_tracy;
    };

}