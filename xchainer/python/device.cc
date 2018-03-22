#include "xchainer/python/device.h"

#include <memory>
#include <sstream>
#include <string>

#include "xchainer/backend.h"
#include "xchainer/context.h"
#include "xchainer/device.h"

#include "xchainer/python/common.h"

namespace xchainer {
namespace python {
namespace internal {

namespace py = pybind11;  // standard convention

class PyDeviceScope {
public:
    explicit PyDeviceScope(Device& target) : target_(target) {}
    void Enter() { scope_ = std::make_unique<DeviceScope>(target_); }
    void Exit(py::args args) {
        (void)args;  // unused
        scope_.reset();
    }

private:
    // TODO(beam2d): better to replace it by "optional"...
    std::unique_ptr<DeviceScope> scope_;
    Device& target_;
};

void InitXchainerDevice(pybind11::module& m) {
    py::class_<Device>(m, "Device")
            .def("__repr__", &Device::name)
            .def("synchronize", &Device::Synchronize)
            .def_property_readonly("name", &Device::name)
            .def_property_readonly("backend", &Device::backend, py::return_value_policy::reference)
            .def_property_readonly("context", &Device::context, py::return_value_policy::reference)
            .def_property_readonly("index", &Device::index);

    m.def("get_default_device", []() -> Device& { return GetDefaultDevice(); }, py::return_value_policy::reference);
    m.def("set_default_device", [](Device& device) { SetDefaultDevice(&device); });
    m.def("set_default_device", [](const std::string& device_name) { SetDefaultDevice(&GetDefaultContext().GetDevice(device_name)); });

    py::class_<PyDeviceScope>(m, "DeviceScope").def("__enter__", &PyDeviceScope::Enter).def("__exit__", &PyDeviceScope::Exit);
    m.def("device_scope", [](Device& device) { return PyDeviceScope(device); });
    m.def("device_scope", [](const std::string& device_name) { return PyDeviceScope(GetDefaultContext().GetDevice(device_name)); });
    m.def("device_scope", [](const std::string& backend_name, int index) {
        return PyDeviceScope(GetDefaultContext().GetDevice({backend_name, index}));
    });
}

}  // namespace internal
}  // namespace python
}  // namespace xchainer
