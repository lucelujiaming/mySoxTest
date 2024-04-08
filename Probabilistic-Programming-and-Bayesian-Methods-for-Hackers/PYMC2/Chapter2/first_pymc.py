import pymc as pm

with pm.Model() as model:
    lambda_ = pm.Exponential("poisson_param", 1)
    # used in the call to the next variable...
    data_generator = pm.Poisson("data_generator", lambda_)
    data_plus_one = data_generator + 1

# print("Children of 'lambda_': ")
# print(lambda_.children)
# print("\nParents of 'data_generator': ")
# print(data_generator.parents)
# print("\nChildren of 'data_generator': ")
# print(data_generator.children)

# print("lambda_.value =", lambda_.value)
# print("data_generator.value =", data_generator.value)
# print("data_plus_one.value =", data_plus_one.value)

print("lambda_.value =", lambda_)
print("data_generator.value =", data_generator)
print("data_plus_one.value =", data_plus_one)

with pm.Model() as model:
    some_variable = pm.DiscreteUniform("discrete_uni_var", 0, 4)

print("some_variable =", some_variable)

