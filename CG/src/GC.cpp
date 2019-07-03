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

    lambdaItens = std::vector<std::vector<int>>(data.getNItems(), {1});
    for (int i = 0; i < data.getNItems(); i++)
    {
        lambdaItens[i][0] = i;
    }

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

        somaPricing.end();

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
            std::vector<int> itens;
            for (int i = 0; i < x_vals.getSize(); i++)
            {
                if (x_vals[i] > 1 - EPSILON)
                {
                    lambdaItens[i].push_back(lambda.getSize() - 1);
                }
            }
            try
            {
                master.solve();
            }
            catch (IloException &e)
            {
                std::cout << e;
            }

            x_vals.end();
        }
        else
        {
            x_vals.end();

            break;
        }
    }

    for (int i = 0; i < lambda.getSize(); i++)
    {
        std::cout << i << ", " << master.getValue(lambda[i]) << "\n";
    }

    std::cout << "Status: " << master.getStatus() << "\n";
    std::cout << "Bins usados: " << master.getObjValue() << "\n\n\n\n\n";

    //branching rule

    IloNumArray lambdaVals(env, lambda.getSize());
    master.getValues(lambdaVals, lambda);
    double deltaFrac = std::numeric_limits<double>::infinity();
    int lambdaBranch;
    double tempDeltaFrac;

    std::vector<double> lambdaPares((data.getNItems() * data.getNItems() - data.getNItems()) / 2);

    for (int i = 0; i < data.getNItems(); i++)
    {
        for (int j = i + 1; j < data.getNItems(); j++)
        {
            
        }
    }

    for (int i = data.getNItems(); i < lambdaVals.getSize(); i++)
    {
        tempDeltaFrac = std::abs(0.5 - lambdaVals[i]);
        if (tempDeltaFrac < deltaFrac)
        {
            lambdaBranch = i;
            deltaFrac = tempDeltaFrac;
        }
    }

    for (int i = 0; i < data.getNItems(); i++)
    {
    }

    lambdaVals.end();

    std::cout << lambdaBranch << ", " << deltaFrac << "\n";

    for (int i = 0; i < lambdaItens.size(); i++)
    {
        for (int j = 0; j < lambdaItens[i].size(); j++)
        {
            std::cout << lambdaItens[i][j] << " ";
        }
        std::cout << "\n";
    }

    master.exportModel("modelo.lp");
}
