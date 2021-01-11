from .utils import CompileError
from .utils import is_primitive_type
from .utils import is_snake_case
from .utils import split_dict_mys_type


class Context:
    """The context keeps track of defined functions, classes, traits,
    enums and variables in the current scope. Ot also provides other
    services, like unique numbers and full name convertions.

    """

    def __init__(self,
                 module_levels='',
                 specialized_functions=None,
                 specialized_classes=None):
        self.name = '.'.join(module_levels)
        self._stack = [[]]
        self._local_variables = {}
        self._global_variables = {}
        self._classes = {}
        self._traits = {}
        self._functions = {}
        self._enums = {}
        self.return_mys_type = None
        self.mys_type = None
        self.unique_count = 0
        self.constants = {}
        self._name_to_full_name ={}
        self.specialized_functions = specialized_functions
        self.specialized_classes = specialized_classes

    def unique_number(self):
        self.unique_count += 1

        return self.unique_count

    def unique(self, name):
        return f'__{name}_{self.unique_number()}'

    def make_full_name(self, name):
        """Returns the fully qualified name (full_name) of given function,
        class, trait, enum or global variable name.

        """

        return self._name_to_full_name.get(name)

    def define_local_variable(self, name, mys_type, node):
        if self.is_local_variable_defined(name):
            raise CompileError(f"redefining variable '{name}'", node)

        if not is_snake_case(name):
            raise CompileError("local variable names must be snake case", node)

        if name in self._name_to_full_name:
            raise CompileError(f"redefining '{name}'", node)

        self._local_variables[name] = mys_type
        self._stack[-1].append(name)

    def is_local_variable_defined(self, name):
        """Returns true if given short local variable name is defined.

        """

        return name in self._local_variables

    def get_local_variable_type(self, name):
        return self._local_variables[name]

    def define_global_variable(self, name, full_name, mys_type, _node):
        self._global_variables[name] = mys_type
        self._name_to_full_name[name] = full_name

    def is_global_variable_defined(self, name):
        """Returns true if given name short global variable name is defined.

        """

        return name in self._global_variables

    def get_global_variable_type(self, name):
        return self._global_variables[name]

    def make_full_name_this_module(self, name):
        """Returns the fully qualified name (full_name) of given name as
        defined in the current module.

        """

        return f'{self.name}.{name}'

    def define_class(self, name, full_name, definitions):
        self._name_to_full_name[name] = full_name
        self._classes[full_name] = definitions

    def is_class_defined(self, name):
        """Returns true if given type is a class. Accepts both short names and
        fully qualified names.

        """

        if not isinstance(name, str):
            return False

        full_name = self._name_to_full_name.get(name, name)

        return full_name in self._classes

    def get_class_definitions(self, name):
        full_name = self._name_to_full_name.get(name, name)

        return self._classes[full_name]

    def define_trait(self, name, full_name, definitions):
        self._name_to_full_name[name] = full_name
        self._traits[full_name] = definitions

    def is_trait_defined(self, name):
        """Returns true if given type is a trait. Accepts both short names and
        fully qualified names.

        """

        if not isinstance(name, str):
            return False

        full_name = self._name_to_full_name.get(name, name)

        return full_name in self._traits

    def get_trait_definitions(self, name):
        full_name = self._name_to_full_name.get(name, name)

        return self._traits[full_name]

    def define_function(self, name, full_name, definitions):
        self._name_to_full_name[name] = full_name
        self._functions[full_name] = definitions

    def is_function_defined(self, full_name):
        """Returns true if given fully qualified function name is defined.

        """

        return full_name in self._functions

    def get_functions(self, full_name):
        return self._functions[full_name]

    def define_enum(self, name, full_name, type_):
        self._name_to_full_name[name] = full_name
        self._enums[full_name] = type_

    def is_enum_defined(self, name):
        """Returns true if given type is an enum. Accepts both short names and
        fully qualified names.

        """

        if not isinstance(name, str):
            return False

        full_name = self._name_to_full_name.get(name, name)

        return full_name in self._enums

    def get_enum_type(self, name):
        full_name = self._name_to_full_name.get(name, name)

        return self._enums[full_name]

    def is_class_or_trait_defined(self, full_name):
        if self.is_class_defined(full_name):
            return True
        elif self.is_trait_defined(full_name):
            return True

        return False

    def is_type_defined(self, mys_type):
        if isinstance(mys_type, tuple):
            for item_mys_type in mys_type:
                if not self.is_type_defined(item_mys_type):
                    return False
        elif isinstance(mys_type, list):
            for item_mys_type in mys_type:
                if not self.is_type_defined(item_mys_type):
                    return False
        elif isinstance(mys_type, dict):
            key_mys_type, value_mys_type = split_dict_mys_type(mys_type)

            if not self.is_type_defined(key_mys_type):
                return False

            if not self.is_type_defined(value_mys_type):
                return False
        elif self.is_class_or_trait_defined(mys_type):
            return True
        elif self.is_enum_defined(mys_type):
            return True
        elif is_primitive_type(mys_type):
            return True
        elif mys_type == 'string':
            return True
        elif mys_type == 'bytes':
            return True
        elif mys_type is None:
            return True
        else:
            return False

        return True

    def define_specialized_function(self, full_name, function):
        self.specialized_functions[full_name] = (function, set())

    def add_self_as_specialized_function_caller(self, full_name):
        self.specialized_functions[full_name][1].add(self.name)

    def is_specialized_function_defined(self, full_name):
        return full_name in self.specialized_functions

    def get_specialized_function(self, full_name):
        return self.specialized_functions[full_name][0]

    def define_specialized_class(self, full_name, definitions):
        self.specialized_classes[full_name] = (definitions, set())

    def add_self_as_specialized_class_caller(self, full_name):
        self.specialized_classes[full_name][1].add(self.name)

    def is_specialized_class_defined(self, full_name):
        return full_name in self.specialized_classes

    def get_specialized_class(self, full_name):
        return self.specialized_classes[full_name][0]

    def push(self):
        self._stack.append([])

    def pop(self):
        result = {}

        for name in self._stack[-1]:
            result[name] = self._local_variables.pop(name)

        self._stack.pop()

        return result