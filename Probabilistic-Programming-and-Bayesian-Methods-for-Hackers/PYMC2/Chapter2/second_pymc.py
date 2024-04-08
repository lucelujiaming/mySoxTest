import pymc as pm

with pm.Model() as model:
    lambda_1 = pm.Exponential("lambda_1", 1) # prior on first behavior
    lambda_2 = pm.Exponential("lambda_2", 1) # prior on second behavior
    tau = pm.DiscreteUniform("tau", lower=0, upper=10) # prior on behavior change

print("Initialized values...")
print("lambda_1.value: ", lambda_1)
print("lambda_2.value: ", lambda_2)
print("tau.value: ", tau)
print(pm.draw(lambda_2), pm.draw(lambda_2), pm.draw(tau))
print("After calling random() on the variables...")
print("lambda_1.value: ", lambda_1)
print("lambda_2.value: ", lambda_2)
print("tau.value: ", tau)

