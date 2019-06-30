#include "GC.h"

#define EPSILON 0.000001

void solve(Data &data, std::vector<std::vector<int>> &patterns)
{
    IloEnv env;
    IloModel master(env);
    IloNumVarArray lambda(env, data.getNItems(), 0.0, 1.0);
    IloRangeArray constraints(env);

    IloExpr sum(env);
    for (int i = 0; i < data.getNItems(); i++)
    {
        sum += lambda[i];
        constraints.add(lambda[i] == 1);
    }

    master.add(constraints);
    IloObjective masterObj(env);
    masterObj = IloMinimize(env, sum);
    master.add(masterObj);

    IloCplex BPP(master);

    IloNumArray pi(env);

    try
    {
        BPP.solve();
    }

    catch (IloException &e)
    {
        std::cout << e;
    }

    std::cout << "Status: " << BPP.getStatus() << "\n";
    std::cout << "Bins usados: " << BPP.getObjValue() << "\n";

    int k = 1;

    IloModel subproblem(env);
    IloBoolVarArray x(env, data.getNItems());
    IloExpr somaItems(env);
    for (int i = 0; i < data.getNItems(); i++)
    {
        somaItems += data.getItemWeight(i) * x[i];
    }
    subproblem.add(somaItems <= data.getBinCapacity());
    IloObjective subObj(env);
    subproblem.add(subObj);

    while (1)
    {
        BPP.getDuals(pi, constraints);

        IloExpr somaPricing(env);
        for (int i = 0; i < data.getNItems(); i++)
        {
            somaPricing += pi[i] * x[i];
        }
        subObj.setExpr(1 - somaPricing);

        IloCplex pricing(subproblem);

        try
        {
            pricing.solve();
        }

        catch (IloException &e)
        {
            std::cout << e;
        }
        // std::cout << "Status: " << pricing.getStatus() << "\n";
        // std::cout << "Bins usados: " << pricing.getObjValue() << "\n";

        // for (int i = 0; i < data.getNItems(); i++)
        // {
        //     std::cout << pricing.getValue(x[i]) << "\n";
        // }

        IloNumArray x_vals(env, data.getNItems());
        pricing.getValues(x_vals, x);

        if (pricing.getObjValue() < -EPSILON)
        {
            lambda.add(IloNumVar(masterObj(1) + constraints(x_vals), 0.0, 1.0));
            // master.add();

            try
            {
                BPP.solve();
            }
            catch (IloException &e)
            {
                std::cout << e;
            }

            std::cout << "Status: " << BPP.getStatus() << "\n";
            std::cout << "Bins usados: " << BPP.getObjValue() << "\n\n\n\n\n";
        }
        else
        {
            break;
        }
    }

    for (int i = 0; i < lambda.getSize(); i++)
    {
        std::cout << BPP.getValue(lambda[i]) << "\n";
    }
    env.end();
}
