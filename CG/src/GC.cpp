#include "GC.h"

Problema::Problema(Data &d)
{
    //Construçao do mestre

    this->data = d;

    env = IloEnv();
    masterModel = IloModel(env);
    lambda = IloNumVarArray(env, data.getNItems(), 0.0, IloInfinity);
    masterRanges = IloRangeArray(env);

    IloExpr sum(env);
    for (int i = 0; i < data.getNItems(); i++)
    {
        sum += lambda[i];
        masterRanges.add(lambda[i] == 1);
    }

    masterModel.add(masterRanges);
    masterObj = IloObjective(env);
    masterObj = IloMinimize(env, sum);
    masterModel.add(masterObj);

    master = IloCplex(masterModel);

    pi = IloNumArray(env, data.getNItems());

    pricingModel = IloModel(env);
    x = IloBoolVarArray(env, data.getNItems());
    IloExpr somaItems(env);

    for (int i = 0; i < data.getNItems(); i++)
    {
        somaItems += data.getItemWeight(i) * x[i];
    }

    pricingModel.add(somaItems <= data.getBinCapacity());
    pricingObj = IloObjective(env);
    pricingModel.add(pricingObj);

    pricing = IloCplex(pricingModel);

    try
    {
        master.solve();
    }

    catch (IloException &e)
    {
        std::cout << e;
    }

    for (int i = 0; i < lambda.getSize(); i++)
    {
        std::cout << master.getValue(lambda[i]) << "\n";
    }

}

void Problema::solve()
{

    while (1)
    {
        master.getDuals(pi, masterRanges);

        IloExpr somaPricing(env);

        for (int i = 0; i < data.getNItems(); i++)
        {
            somaPricing += pi[i] * x[i];
        }

        pricingObj.setExpr(1 - somaPricing);

        IloCplex pricing(pricingModel);

        try
        {
            pricing.solve();
        }

        catch (IloException &e)
        {
            std::cout << e;
        }

        IloNumArray x_vals(env, data.getNItems());
        pricing.getValues(x_vals, x);

        if (pricing.getObjValue() < -EPSILON)
        {
            lambda.add(IloNumVar(masterObj(1) + masterRanges(x_vals), 0.0, IloInfinity));

            try
            {
                master.solve();
            }
            catch (IloException &e)
            {
                std::cout << e;
            }
        }
        else
        {
            break;
        }
    }

    for (int i = 0; i < lambda.getSize(); i++)
    {
        std::cout << master.getValue(lambda[i]) << "\n";
    }

    std::cout << "Status: " << master.getStatus() << "\n";
    std::cout << "Bins usados: " << master.getObjValue() << "\n\n\n\n\n";

    master.exportModel("modelo.lp");

}
