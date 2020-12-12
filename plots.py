import plotly.express as px
import pandas

dfNoMigration = pandas.read_csv("no-migration.csv").sort_values(by="Completion Time")
dfFrontMigration = pandas.read_csv("front-migration.csv").sort_values(by="Completion Time")
dfBackMigration = pandas.read_csv("back-migration.csv").sort_values(by="Completion Time")

dfCompletionTime = pandas.DataFrame.from_dict({
    'No Migration' : dfNoMigration['Completion Time'],
    'Front Migration' : dfFrontMigration['Completion Time'],
    'Back Migration' : dfBackMigration['Completion Time']
})

fig = px.line(dfCompletionTime, x=dfCompletionTime.index, y=[dfNoMigration['Completion Time'], dfFrontMigration['Completion Time'], dfBackMigration['Completion Time']])
fig.show()

dfDict = {
    'Migrated' : [dfNoMigration['Times Migrated'].sum(), dfFrontMigration['Times Migrated'].sum(), dfBackMigration['Times Migrated'].sum()],
    'Restarted' : [dfNoMigration['Times Restarted'].sum(), dfFrontMigration['Times Restarted'].sum(), dfBackMigration['Times Restarted'].sum()]
}

df = pandas.DataFrame.from_dict(dfDict)

fig = px.bar(df, x=df.index, y=['Migrated', 'Restarted'], barmode='group', labels={'0':'No Migration', '1':'Front Migration', '2':'Back Migration'})
fig.show()