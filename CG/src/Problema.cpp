#include "Problema.h"

Problema::Problema(Data &d)
{
    //Construçao do mestre

    this->data = d;

    env1 = IloEnv();
    env2 = IloEnv();
    masterModel = IloModel(env1);
    lambda = IloNumVarArray(env1, data.getNItems(), 0.0, IloInfinity);
    masterRanges = IloRangeArray(env1);

    IloExpr sum(env1);
    for (int i = 0; i < data.getNItems(); i++)
    {
        sum += lambda[i];
        masterRanges.add(lambda[i] == 1);
    }

    masterModel.add(masterRanges);
    masterObj = IloObjective(env1);
    masterObj = IloMinimize(env1, sum);
    masterModel.add(masterObj);

    master = IloCplex(masterModel);

    pi = IloNumArray(env1, data.getNItems());

    pricingModel = IloModel(env2);
    x = IloBoolVarArray(env2, data.getNItems());
    IloExpr somaItems(env2);

    for (int i = 0; i < data.getNItems(); i++)
    {
        somaItems += data.getItemWeight(i) * x[i];
    }

    pricingModel.add(somaItems <= data.getBinCapacity());
    pricingObj = IloObjective(env2);
    pricingModel.add(pricingObj);

    pricing = IloCplex(pricingModel);

    lambdaItens = std::vector<std::vector<int>>(data.getNItems(), {1});

    pricing.setOut(env2.getNullStream());
    master.setOut(env1.getNullStream());

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

    // for (int i = 0; i < lambda.getSize(); i++)
    // {
    //     std::cout << master.getValue(lambda[i]) << "\n";
    // }
}

std::pair<int, int> Problema::solve(Node &node)
{
    //tratamento de nos juntos
    if (!node.juntos.empty())
    {
        int i = node.juntos[node.juntos.size() - 1].first;
        int j = node.juntos[node.juntos.size() - 1].second;

        x[i].setLB(1.0);
        x[j].setLB(1.0);
    }

    //tratamento de nos separados
    if (!node.separados.empty())
    {
        int i = node.separados[node.separados.size() - 1].first;
        int j = node.separados[node.separados.size() - 1].second;

        pricingModel.add(x[i] + x[j] <= 1);

        for (int k = data.getNItems(); k < lambdaItens.size(); k++)
        {
            for (int l = 0; l < lambdaItens[k].size() - 1; l++)
            {
                if (lambdaItens[k][l] == 1 && lambdaItens[k][l + 1] == 1)
                {
                    lambda[k].setUB(0.0);
                }
            }
        }
    }

    xPares = std::vector<std::vector<double>>(data.getNItems(), std::vector<double>(data.getNItems(), 0));

    while (1)
    {
        master.getDuals(pi, masterRanges);

        IloExpr somaPricing(env2);

        for (int i = 0; i < data.getNItems(); i++)
        {
            somaPricing += pi[i] * x[i];
        }

        pricingObj.setExpr(1 - somaPricing);

        somaPricing.end();

        IloCplex pricing(pricingModel);

        pricing.setOut(env2.getNullStream());
        try
        {
            pricing.solve();
        }

        catch (IloException &e)
        {
            std::cout << e;
        }

        IloNumArray x_vals(env2, data.getNItems());
        pricing.getValues(x_vals, x);

        if (pricing.getObjValue() < -EPSILON)
        {
            lambda.add(IloNumVar(masterObj(1) + masterRanges(x_vals), 0.0, IloInfinity));

            std::vector<int> itens;

            std::cout << "\n\n\nitens pricing : ";
            for (int i = 0; i < x_vals.getSize(); i++)
            {
                if (x_vals[i] > 1 - EPSILON)
                {
                    std::cout << i << " ";
                    itens.push_back(i);
                }
            }
            std::cout << "\n\n\n";

            lambdaItens.push_back(itens);

            master.setOut(env1.getNullStream());
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

    IloNumArray lambdaVals(env1, lambda.getSize());
    master.getValues(lambdaVals, lambda);
    double deltaFrac = std::numeric_limits<double>::infinity();
    double tempDeltaFrac;

    std::vector<double> lambdaPares((data.getNItems() * data.getNItems() - data.getNItems()) / 2);

    for (int i = data.getNItems(); i < lambdaItens.size(); i++)
    {
        for (int j = 0; j < lambdaItens[i].size() - 1; j++)
        {
            xPares[lambdaItens[i][j]][lambdaItens[i][j + 1]] = (xPares[lambdaItens[i][j + 1]][lambdaItens[i][j]] += lambdaVals[i]);
        }
    }

    std::pair<int, int> branchingPair;

    for (int i = 0; i < data.getNItems(); i++)
    {
        for (int j = i + 1; j < data.getNItems(); j++)
        {
            tempDeltaFrac = std::abs(0.5 - xPares[i][j]);
            if (tempDeltaFrac < deltaFrac)
            {
                branchingPair = {i, j};
                deltaFrac = tempDeltaFrac;
            }
        }
    }

    // if()

    lambdaVals.end();

    master.exportModel("modelo.lp");
    pricing.exportModel("pricing.lp");

    std::cout << branchingPair.first << ", " << branchingPair.second << "\n";

    return branchingPair;
}
